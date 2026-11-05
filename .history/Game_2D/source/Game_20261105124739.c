#include "Game.h"

void initializeGame(Game* game){//réalise toutes les initialisations possibles et necessaires pour le jeu
	int i = 0;
	romfsInit();//utile pour la gestion des sprites
	cfguInit();//active les trucs necessaires pour le rendu de texte
	gfxInitDefault();//Initialise les framebuffers du jeu avec leur valeur par défaut
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);//on initialise la citro3D
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);//on initialise auusi la citro2D et pensant en paramètres le nombre maximale d'objets qui peuvent être dessinés par frame donc on passe ici en paramètre la valeur maximale par défaut
	C2D_Prepare();//preparer le GPU pour du dessiner du rendu 2D

	//Maintenant on va créer les écrans :
	game->topScreen  = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);//on créé le renderer pour la fenêtre du haut
	game->bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	//initialisation des valeurs du timers
	game->fps = 33.0f;
	game->actualTimer = 0.0f;
	game->afterTimer = 0.0f;

	//On va maintenant charger les sprites du jeu
	game->spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");//on charge le spritesheet du jeu
	if(!game->spriteSheet) svcBreak(USERBREAK_PANIC);//si on a pas pu charger le spritesheet on stoppe le jeu

	//Maintenant les initialisations pour la gestion du texte
	game->textBuffer = C2D_TextBufNew(4096);//un maximim de glyph par buffer (en vrai j'ai pas compris)
	//textFont = C2D_FontLoad();//on charge la police du jeu
	game->defaultFont = C2D_FontLoadSystem(CFG_REGION_EUR);
	if(!game->defaultFont){//si on a pas réussi à charger la texture
		game->defaultFont = NULL;
	}
	//on va charger une police personnalisé
	game->poetseneFont = C2D_FontLoad("romfs:/gfx/PoetsenOne-Regular.bcfnt");
	if(!game->poetseneFont){//si on a pas réussi à charger cette texture donc par défaut on prend la police système
		game->poetseneFont = NULL;
	}
	
	//on va spécifier quelques informations pour un texte
	setTextPosition(&game->helloWorld, 10.0f, 150.0f);//définit la position de helloworld
	setTextScale(&game->helloWorld, 0.5f, 0.5f);//on définit la dimension du texte
	setTextColor(&game->helloWorld, C2D_Color32(255, 255, 255, 255));//maintenant la couleur

	//Maintenant chargons les textures du jeu
	loadSprite(&game->bird[0], game->spriteSheet, 2);//on charge le bird en mémoire
	loadSprite(&game->bird[1], game->spriteSheet, 3);//on charge le bird en mémoire
	loadSprite(&game->bird[2], game->spriteSheet, 4);//on charge le bird en mémoire
	setSpritePosition(&game->bird[0], 50, 0);//on définit la position du bird
	loadSprite(&game->background, game->spriteSheet, 0);//on charge l'arrière plan du jeu
	setSpritePosition(&game->background, 0, 0);//on définit la position du background
	loadSprite(&game->base, game->spriteSheet, 1);//on charge la base du jeu
	setSpritePosition(&game->base, 0, 188);//on définit la position de la base

	//initialise certaines variables du jeu
	game->gravite = 0.5f;
	game->vitesse = 0.5f;
	game->impulsion = -8.0f;
	game->animationBird = 1;//indique quel bird il faut dessiner
	game->angle = 4;//angle de rotation pour le bird
	game->ecartXBetweenPipe = 120;//les écarts entre les pipes horizontalement
	game->ecartYBetweenPipe = 150 + 46;// comme on a choisi le centre des images comme origine prendre en compte les 2 moitiés
	game->min = -60;//la position minimale d'un pipe du haut
	game->max = 50;//la position maximale en y d'un pipe du bas
	game->score = 0;//le score du jeu
	game->lastPipeX = 400;//on place le premier pipe à la toute fin
	game->birdY = 0;//l'ordonnée commune des birds
	game->animationBirdAfterTimer = 0;//utilisé pour la gestion de l'animation
	game->birdAnimationFps = 120;//le nombre de fps pour jouer une animation

	//on place les pipes
	for(i = 0; i < 4; i++){
		loadSprite(&game->upPipes[i], game->spriteSheet, 5);//on charge les pipes du haut
		C2D_SpriteSetCenter(&game->upPipes[i].sprite, 0.5f, 0.5f);//mettre l'origine de l'image à son centre pour la rotation
		setSpritePosition(&game->upPipes[i], game->lastPipeX + game->ecartXBetweenPipe, game->min + rand() % (game->max - game->min));//on définit la position du pipe du haut
		C2D_SpriteRotateDegrees(&game->upPipes[i].sprite, 180.0f);//on tourne de 90 degre les pipes
		loadSprite(&game->downPipes[i], game->spriteSheet, 5);//on charge les pipes du bas
		C2D_SpriteSetCenter(&game->downPipes[i].sprite, 0.5f, 0.5f);
		setSpritePosition(&game->downPipes[i], game->lastPipeX + game->ecartXBetweenPipe, game->upPipes[i].y + game->ecartYBetweenPipe);//la position des pipes du bas
		game->lastPipeX += game->ecartXBetweenPipe;//on met à jour la position du dernier pipe
	}
}

int  handleEvents(Game* game){
	hidScanInput();//on recupère les inputs du joueur, scan les inputs du joueur

	// Your code goes here
	game->kDown = hidKeysDown();//on recupère la touche préssée par le joueur
	if (game->kDown & KEY_START){//si le joueur a appuyé sur la touche start on quitte la boucle du jeu
		return 0;//on retourne false donc on doit quitter le jeu
	}
	if(game->kDown & KEY_LEFT){//si le joueur appuie sur la touche directionnelle de gauche
		//moveSprite(&game->bird, -5, 0);//on va déplacer vers la gauche
	}
	if(game->kDown & KEY_RIGHT){//le joueur appuie sur la touche directionnelle de droite
		//moveSprite(&game->bird, 5, 0);//on va déplacer vers la droite
	}
	if(game->kDown & KEY_UP){//le joueur appuie sur la touche directionnelle du haut
		//moveSprite(&game->bird, 0, 5);//on va déplacer vers le haut
	}
	if(game->kDown & KEY_DOWN){//le joueur appuie sur la touche directionnelle du bas
		//moveSprite(&game->bird, 0, -5);//on va déplacer vers le bas
	}

	//sinon par défaut on retourne true
	return 1;//vrai
}

void render2D(Game* game){//méthode de rendu 2D
	int i = 0;
	C3D_LightEnvBind(NULL);//je désactive l'éclairage pour la 2D
	C2D_TargetClear(game->topScreen, C2D_Color32(0, 0, 0, 0));//permet de peindre l'écran du haut avec une couleur bien définie
	C2D_SceneBegin(game->topScreen);//on commence à dessiner la scene 2D sur le topScreen

	//on va dessiner les éléments ici, on doit toujours dessiner les objets circulaires à la fin

	drawSprite(&game->background);
	for(i = 0; i < 4; i++){
		drawSprite(&game->upPipes[i]);//on dessine les pipes du haut, on dessine les pipes du bas
		drawSprite(&game->downPipes[i]);
	}
	drawSprite(&game->base);
	if(game->animationBird == 0){
		drawSprite(&game->bird[0]);
	}
	else if(game->animationBird == 1){
		drawSprite(&game->bird[1]);
	}
	else if(game->animationBird == 2){
		drawSprite(&game->bird[2]);
	}
	
	C2D_TextBufClear(game->textBuffer);//on reset le texte buffer
	//drawText(&game->helloWorld, game->textBuffer, game->poetseneFont, "salut Josaphat");
}

void renderGame(Game* game){
	
	//le rendu graphique
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);//commence le frame pour la 3D

	//maintenant la 2D
	render2D(game);

	C3D_FrameEnd(0);//On a fini de dessiner la scene
}

void updateGame(Game* game){
	//on met à jour le jeu
	int i  = 0;
	game->actualTimer = svcGetSystemTick();//on recupère la valeur du temps actuel
	if(game->actualTimer - game->afterTimer >= game->fps){
		//on va commencer par coulisser la base
		moveSprite(&game->base, -1.0f, 0.0f);
		if(game->base.x <= -400){
			setSpritePosition(&game->base, 0, 188);
		}

		//mise à jour des pipes
		for(i = 0; i < 4; i++){
			moveSprite(&game->upPipes[i], -1, 0);//on déplace le pipe du haut
			moveSprite(&game->downPipes[i], -1, 0);//on déplace le pipe du bas
			if(game->upPipes[i].x <= -23){//si le pipe passe du côté gauche de la fenêtre
				//essayons d'abord de déterminer la dernière position du l
				if(i == 0){//Donc s'il s'agit du premier pipe
					game->lastPipeX = game->upPipes[3].x;//on recupère la position du quatrième pipe
				}
				else{//sinon c'est les autres pipes
					game->lastPipeX = game->upPipes[i - 1].x;//on recupère la position du dernier pipe et c'est fini
				}
				setSpritePosition(&game->upPipes[i], game->lastPipeX + game->ecartXBetweenPipe, game->min + rand() % (game->max - game->min));//on définit la position du pipe du haut
				setSpritePosition(&game->downPipes[i], game->lastPipeX + game->ecartXBetweenPipe, game->upPipes[i].y + game->ecartYBetweenPipe);//la position des pipes du bas
			}
		}
		//mise à jour de l'animation du bird
		game->animationBird = (game->animationBird + 1) % 3;
		game->afterTimer = game->actualTimer;//on met à jour
	}
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
		updateGame(game);//met à jour le jeu
	}
}


void exitGame(Game* game){//execute toutes les instructions relatif à la fermeture du jeu

	//deinit les libs et libérer la mémoire

	C2D_TextBufDelete(game->textBuffer);//on efface le TextBuffer de la mémoire
	C2D_FontFree(game->defaultFont);//même chose pour la police du texte aussi
	C2D_FontFree(game->poetseneFont);//même chose pour la police du texte aussi

	C2D_Fini();//On quitte Citro2D
	C3D_Fini();//on quitte citro3D
	gfxExit();//on libère la mémoire et on quitte le jeu
	romfsExit();//on exit ce truc aussi
	cfguExit();//on quitte ça aussi
}