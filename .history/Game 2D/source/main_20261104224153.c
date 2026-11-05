#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>//pour gestion du rendu 2D
#define SCREEN_WIDTH 400 //la largeur de la fenêtre
#define SCREEN_HEIGHT 240 //la hauteur de la fenêtre
#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

//les variables globales du jeu
C3D_RenderTarget* bottomScreen = NULL; //pointeur vers l'écran du bas de la 3DS
C3D_RenderTarget* topscreen  = NULL;//pointeur vers l'écran du haut de la 3DS
u32 whiteColor;//la couleur blanche
u32 blackColor;//la couleur noire
u32 kDown;//les inputs du joueur
static C2D_SpriteSheet spriteSheet;//on définit un sprite sheet contenant tous les images dont on a besoin
C2D_TextBuf textBuffer;//on définit le texte Buffer
C2D_Text message;//symbolise le texte qui sera affiché
C2D_Font textFont;//la police du texte qui sera affiché

//des structures utiles dans le jeu
typedef struct{ //On crée une structure pour répresenter un Sprite
	C2D_Sprite sprite;//d'abord un sprite
	float dx, dy; //pour la gestion des déplacements suivant x et y
	float angle;//pour la gestion de la rotation par exemple
}Sprite;

Sprite bird;//on créé notre tout premier sprite qui va représenter le bird

void initializeGame(){//réalise toutes les initialisations possibles et necessaires pour le jeu

	romfsInit();//utile pour la gestion des sprites
	cfguInit();//active les trucs necessaires pour le rendu de texte
	gfxInitDefault();//Initialise les framebuffers du jeu avec leur valeur par défaut
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);//on initialise la citro3D
	gfxSet3D(true);//on active la 3D stereostopic
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);//on initialise auusi la citro2D et pensant en paramètres le nombre maximale d'objets qui peuvent être dessinés par frame donc on passe ici en paramètre la valeur maximale par défaut
	C2D_Prepare();//preparer le GPU pour du dessiner du rendu 2D

	//Maintenant on va créer les écrans :
	topscreen  = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);//on créé le renderer pour la fenêtre du haut
	bottomScreen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	C3D_RenderTargetSetOutput(topscreen,  GFX_TOP, GFX_LEFT,  DISPLAY_TRANSFER_FLAGS);

	//On va maintenant charger les sprites du jeu
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");//on charge le spritesheet du jeu
	if(!spriteSheet) svcBreak(USERBREAK_PANIC);//si on a pas pu charger le spritesheet on stoppe le jeu

	//Maintenant chargons les textures du jeu
	C2D_SpriteFromSheet(&bird.sprite, spriteSheet, 0);//on va charger la première texture
	C2D_SpriteSetCenter(&bird.sprite, 0.0f, 0.0f);//le centre sera le point haut gauche de l'image
	C2D_SpriteSetPos(&bird.sprite, 150, 5);//définit la position du sprite
	bird.dx = 5;//on definit la velocity en x pour le déplacer en x
	bird.dy = 5;//la velocity pour y

	//Maintenant les initialisations pour la gestion du texte
	textBuffer = C2D_TextBufNew(4096);//un maximim de glyph par buffer (en vrai j'ai pas compris)
	//textFont = C2D_FontLoad();//on charge la police du jeu
	textFont = C2D_FontLoadSystem(CFG_REGION_EUR);
	//on charge maintenant le texte à afficher dans le message
	C2D_TextFontParse(&message, textFont, textBuffer, "Salut le monde!");
	//on optimize le texte
	C2D_TextOptimize(&message);//on optimise pour l'affichage du texte

	//Créeons des couleurs
	whiteColor = C2D_Color32(255, 255, 255, 255);//créé une couleur blanche en prenant en paramètre r, g, b, a
	blackColor = C2D_Color32(0, 0, 0, 0);//on crée la couleur noire

}

int  handleEvents(){
	hidScanInput();//on recupère les inputs du joueur, scan les inputs du joueur

	// Your code goes here
	kDown = hidKeysDown();//on recupère la touche préssée par le joueur
	if (kDown & KEY_START){//si le joueur a appuyé sur la touche start on quitte la boucle du jeu
		return 0;//on retourne false donc on doit quitter le jeu
	}
	if(kDown & KEY_LEFT){//si le joueur appuie sur la touche directionnelle de gauche
		angleX--;
	}
	if(kDown & KEY_RIGHT){//le joueur appuie sur la touche directionnelle de droite
		angleX++;
	}
	if(kDown & KEY_UP){//le joueur appuie sur la touche directionnelle du haut
		angleY--;
	}
	if(kDown & KEY_DOWN){//le joueur appuie sur la touche directionnelle du bas
		angleY++;
	}

	//sinon par défaut on retourne true
	return 1;//vrai
}

void render3D(float iod){//on définit la méthode chargé de l'affichage 3D
	//On commence par calculer la matrice de Projection
	Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, iod, 3.0f, false);//matrice de projection
	
	C3D_FVec objectPosition = FVec4_New(0.0f, 0.0f, -3.0f, 1.0f);//on crée un vecteur de float contenant la position de l'objet
	C3D_FVec lightPosition = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);//la source de lumière la position

	//on calcul maintenant la matrice de vue
	C3D_Mtx modelView;//on créé la matrice de vue
	Mtx_Identity(&modelView);//tranforme la matrice de vue en une matrice identité
	Mtx_Translate(&modelView, objectPosition.x, objectPosition.y, objectPosition.z, true);//on fait la translation
	Mtx_RotateX(&modelView, C3D_Angle(sinf(angleX/4)), true);//on applique la rotation suivant l'axe X
	Mtx_RotateY(&modelView, C3D_Angle(angleY), true);//et on applique la rotation suivant Y
	Mtx_Translate(&modelView, 0.0f, -0.5f, 0.0f, true);//on applique une dernière translation

	C3D_LightPosition(&light, &lightPosition);//on définit la position de la lumière

	//on met à jour les uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projectionLocationInShader, &projection);//on charge ou update la valeur de la matrice de projection dans le vertex shader
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, modelViewLocationInShader, &modelView);//on charge aussi le model view

	//Maintenant on dessine le vertex buffer object
	C3D_DrawElements(GPU_TRIANGLES, vertex_element_count, C3D_UNSIGNED_SHORT, teapot_iboData);//on dessine les triangles
	//C3D_DrawElements(GPU_TRIANGLES, arm_vertex_element_count, C3D_UNSIGNED_SHORT, arm_iboData);//on dessine le cube
	//C3D_DrawElements(GPU_TRIANGLES, fox_vertex_element_count, C3D_UNSIGNED_SHORT, fox_iboData);//on dessine fox

}

void render2D(){//méthode de rendu 2D
	C3D_LightEnvBind(NULL);//je désactive l'éclairage pour la 2D
    C2D_Prepare();// je remets l'état citro2d
	C2D_TargetClear(bottomScreen, blackColor);//permet de peindre l'écran du haut avec une couleur bien définie
	C2D_SceneBegin(bottomScreen);//on commence à dessiner la scene 2D sur le topScreen

	//on va dessiner les éléments ici, on doit toujours dessiner les objets circulaires à la fin

	C2D_DrawRectangle(5, 5, 0, 100, 100, whiteColor, whiteColor, whiteColor, whiteColor);//dessiner un rectangle plein de couleur blanche
	C2D_DrawSprite(&bird.sprite);//on dessine maintenant le sprite dans le jeu
	C2D_DrawText(&message, C2D_AtBaseline, 0, 150, 0, 0.5f, 0.5f);//on dessine maintenant le texte à l'écran
}

void renderGame(){
	
	//le rendu graphique
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);//commence le frame pour la 3D
	
	//on dessine maintenant la 3D
	float slider = osGet3DSliderState();
	float iod = slider/2;
	C3D_RenderTargetClear(topscreen, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
	C3D_FrameDrawOn(topscreen);
	bind3DState();//se remmetre en mode 3D
	render3D(iod);

	//maintenant la 2D
	render2D();

	C3D_FrameEnd(0);//On a fini de dessiner la scene
}

void runGameMainLoop(){

	int running = 1;//par défaut vaut vrai et sera utiliser comme bool pour maintenir la boucle principale du jeu ou non
	// la boucle principale du jeu
	while (aptMainLoop())//aptMainLoop retourne true si l'application est toujours en mode running
	{
		renderGame();//on dessine le rendu du jeu
		running = handleEvents();//la gestion des evènements, et stocke la sortie dans running
		if(!running){//si running vaut 0 la condition devient vrai, on quitte donc la boucle principale du jeu
			break;
		}
	}
}

void exit3D(){//libère la mémoire des éléments alloués pour le rendu 3D
	//on commence par vider le vbo
	linearFree(teapot_vboData);
	linearFree(teapot_iboData);
	linearFree(arm_vboData);
	linearFree(arm_iboData);
	linearFree(fox_iboData);
	linearFree(fox_vboData);
	//on libère la mémoire allouée par le program shader
	shaderProgramFree(&programShader);
	//on libère aussi le shader
	DVLB_Free(shader);
}

void exitGame(){//execute toutes les instructions relatif à la fermeture du jeu

	//deinit les libs et libérer la mémoire

	exit3D();//on quitte la 3D

	C2D_TextBufDelete(textBuffer);//on efface le TextBuffer de la mémoire
	C2D_FontFree(textFont);//même chose pour la police du texte aussi

	C2D_Fini();//On quitte Citro2D
	C3D_Fini();//on quitte citro3D
	gfxExit();//on libère la mémoire et on quitte le jeu
	romfsExit();//on exit ce truc aussi
	cfguExit();//on quitte ça aussi
}

int main(int argc, char* argv[])
{
	initializeGame();//on commence par initialiser le jeu
	runGameMainLoop();//on lance la mainLoop du jeu
	exitGame();//on quitte le jeu
	return 0;
}
