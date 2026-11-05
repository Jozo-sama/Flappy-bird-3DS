#ifndef GAME_H
#define GAME_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <3ds.h>
    #include "Sprite.h"//pour gérer les sprites du jeu
    #include "Text.h"//pour gestion de tout ce qui est texte

    typedef struct{//on va créer une structure pour contenir tous les variables necessaires pour la création et le bon déroulement du jeu
        C3D_RenderTarget* bottomScreen; //pointeur vers l'écran du bas de la 3DS
        C3D_RenderTarget* topScreen;//pointeur vers l'écran du haut de la 3DS 
        u32 kDown;//les inputs du joueur
        float fps;//le nombre de frame pas second
        float actualTimer, afterTimer;//utilisé pour les gestion du timer du jeu
        C2D_SpriteSheet spriteSheet;//on définit un sprite sheet contenant tous les images dont on a besoin
        C2D_TextBuf textBuffer;//on définit le texte Buffer
        C2D_Font defaultFont;//la police du texte par défaut du système
        C2D_Font poetseneFont;//une police personnalisée

        //les sprites du jeu
        Sprite bird[3];//on créé notre tout premier sprite qui va représenter le bird
        Sprite background;//on va créer un sprite pour représenter l'arrière plan
        Sprite base;//la base
        Sprite pipe;//les pipelines

        //les textes du jeu à afficher
        Text helloWorld;
    }Game;

    void initializeGame(Game* game);//initialiser le jeu
    int  handleEvents(Game* game);//gestion des inputs du joeur
    void render2D(Game* game);//le rendu 2D du jeu
    void renderGame(Game* game);//rendu globale du jeu
    void runGameMainLoop(Game* game);//la boucle principale du jeu
    void updateGame(Game* game);//on met à jour le jeu
    void exitGame(Game* game);//libérer la mémoire

#endif