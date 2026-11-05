#ifndef GAME_H
#define GAME_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <3ds.h>
    #include "Sprite.h"//pour gérer les sprites du jeu

    typedef struct{//on va créer une structure pour contenir tous les variables necessaires pour la création et le bon déroulement du jeu
        C3D_RenderTarget* bottomScreen; //pointeur vers l'écran du bas de la 3DS
        C3D_RenderTarget* topscreen;//pointeur vers l'écran du haut de la 3DS 
        u32 kDown;//les inputs du joueur
        C2D_SpriteSheet spriteSheet;//on définit un sprite sheet contenant tous les images dont on a besoin
        C2D_TextBuf textBuffer;//on définit le texte Buffer
        C2D_Font textFont;//la police du texte qui sera affiché
        C2D_Text message;//symbolise le texte qui sera affiché
    }Game;

    void initializeGame(Game* game);//initialiser le jeu
    int  handleEvents(Game* game);//gestion des inputs du joeur
    void render2D(Game* game);//le rendu 2D du jeu
    void renderGame(Game* game);//rendu globale du jeu
    void runGameMainLoop(Game* game);//la boucle principale du jeu
    void exitGame(Game* game);//libérer la mémoire

#endif