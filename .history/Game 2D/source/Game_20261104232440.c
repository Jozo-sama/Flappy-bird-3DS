#include "Game.h"

Sprite bird;//on créé notre tout premier sprite qui va représenter le bird
Sprite background;//on va créer un sprite pour représenter l'arrière plan
void initializeGame(Game* game){//réalise toutes les initialisations possibles et necessaires pour le jeu

	romfsInit();//utile pour la gestion des sprites
	cfguInit();//active les trucs necessaires pour le rendu de texte
	gfxInitDefault();//Initialise les framebuffers du jeu avec leur valeur par défaut
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);//on initialise la citro3D
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);//on initialise auusi la citro2D et pensant en paramètres le nombre maximale d'objets qui peuvent être dessinés par frame donc on passe ici en paramètre la valeur maximale par défaut
	C2D_Prepare();//preparer le GPU pour du dessiner du rendu 2D

	//Maintenant on va créer les écrans :
	game->topscreen  = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);//on créé le renderer pour la fenêtre du haut
	game->bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	//On va maintenant charger les sprites du jeu
	game->spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");//on charge le spritesheet du jeu
	if(!game->spriteSheet) svcBreak(USERBREAK_PANIC);//si on a pas pu charger le spritesheet on stoppe le jeu

	//Maintenant chargons les textures du jeu
	C2D_SpriteFromSheet(&bird.sprite, game->spriteSheet, 0);//on va charger la première texture
	C2D_SpriteSetCenter(&bird.sprite, 0.0f, 0.0f);//le centre sera le point haut gauche de l'image
	C2D_SpriteSetPos(&bird.sprite, 150, 5);//définit la position du sprite
	bird.dx = 5;//on definit la velocity en x pour le déplacer en x
	bird.dy = 5;//la velocity pour y

	//Maintenant les initialisations pour la gestion du texte
	game->textBuffer = C2D_TextBufNew(4096);//un maximim de glyph par buffer (en vrai j'ai pas compris)
	//textFont = C2D_FontLoad();//on charge la police du jeu
	game->textFont = C2D_FontLoadSystem(CFG_REGION_EUR);
	//on charge maintenant le texte à afficher dans le message
	C2D_TextFontParse(&game->message, game->textFont, game->textBuffer, "Salut le monde!");
	//on optimize le texte
	C2D_TextOptimize(&game->message);//on optimise pour l'affichage du texte

}

int  handleEvents(Game* game){
	hidScanInput();//on recupère les inputs du joueur, scan les inputs du joueur

	// Your code goes here
	game->kDown = hidKeysDown();//on recupère la touche préssée par le joueur
	if (game->kDown & KEY_START){//si le joueur a appuyé sur la touche start on quitte la boucle du jeu
		return 0;//on retourne false donc on doit quitter le jeu
	}
	if(game->kDown & KEY_LEFT){//si le joueur appuie sur la touche directionnelle de gauche
		//angleX--;
	}
	if(game->kDown & KEY_RIGHT){//le joueur appuie sur la touche directionnelle de droite
		//angleX++;
	}
	if(game->kDown & KEY_UP){//le joueur appuie sur la touche directionnelle du haut
		//angleY--;
	}
	if(game->kDown & KEY_DOWN){//le joueur appuie sur la touche directionnelle du bas
		//angleY++;
	}

	//sinon par défaut on retourne true
	return 1;//vrai
}

void render2D(Game* game){//méthode de rendu 2D
	C3D_LightEnvBind(NULL);//je désactive l'éclairage pour la 2D
	C2D_TargetClear(game->bottomScreen, C2D_Color32(0, 0, 0, 0));//permet de peindre l'écran du haut avec une couleur bien définie
	C2D_SceneBegin(game->bottomScreen);//on commence à dessiner la scene 2D sur le topScreen

	//on va dessiner les éléments ici, on doit toujours dessiner les objets circulaires à la fin
    u32 whiteColor;//la couleur blanche
    whiteColor = C2D_Color32(255, 255, 255, 255);//créé une couleur blanche en prenant en paramètre r, g, b, a
	C2D_DrawRectangle(5, 5, 0, 100, 100, whiteColor, whiteColor, whiteColor, whiteColor);//dessiner un rectangle plein de couleur blanche
	C2D_DrawSprite(&bird.sprite);//on dessine maintenant le sprite dans le jeu
	C2D_DrawText(&game->message, C2D_AtBaseline, 0, 150, 0, 0.5f, 0.5f);//on dessine maintenant le texte à l'écran
}

void renderGame(Game* game){
	
	//le rendu graphique
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);//commence le frame pour la 3D

	//maintenant la 2D
	render2D(game);

	C3D_FrameEnd(0);//On a fini de dessiner la scene
}

void runGameMainLoop(Game* game){

	int running = 1;//par défaut vaut vrai et sera utiliser comme bool pour maintenir la boucle principale du jeu ou non
	// la boucle principale du jeu
	while (aptMainLoop())//aptMainLoop retourne true si l'application est toujours en mode running
	{
		renderGame(game);//on dessine le rendu du jeu
		running = handleEvents(game);//la gestion des evènements, et stocke la sortie dans running
		if(!running){//si running vaut 0 la condition devient vrai, on quitte donc la boucle principale du jeu
			break;
		}
	}
}


void exitGame(Game* game){//execute toutes les instructions relatif à la fermeture du jeu

	//deinit les libs et libérer la mémoire

	C2D_TextBufDelete(game->textBuffer);//on efface le TextBuffer de la mémoire
	C2D_FontFree(game->textFont);//même chose pour la police du texte aussi

	C2D_Fini();//On quitte Citro2D
	C3D_Fini();//on quitte citro3D
	gfxExit();//on libère la mémoire et on quitte le jeu
	romfsExit();//on exit ce truc aussi
	cfguExit();//on quitte ça aussi
}