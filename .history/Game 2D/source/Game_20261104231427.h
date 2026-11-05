#ifndef GAME_H
#define GAME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro2d.h>//pour gestion du rendu 2D

typedef struct{//on va créer une structure pour contenir tous les variables necessaires pour la création et le bon déroulement du jeu
    C3D_RenderTarget* bottomScreen; //pointeur vers l'écran du bas de la 3DS
    C3D_RenderTarget* topscreen;//pointeur vers l'écran du haut de la 3DS 
    u32 kDown;//les inputs du joueur
    C2D_SpriteSheet spriteSheet;//on définit un sprite sheet contenant tous les images dont on a besoin
}Game;

void initializeGame()

#endif