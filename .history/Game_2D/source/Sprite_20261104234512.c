#include "Sprite.h"

void loadSprite(Sprite* sprite, C2D_SpriteSheet spriteSheet, int spriteIndice){//fonction permettant de charger un sprite
    C2D_SpriteFromSheet(&sprite->sprite, spriteSheet, 0);//on va charger la première texture
	C2D_SpriteSetCenter(&sprite->sprite, 0.0f, 0.0f);//le centre sera le point haut gauche de l'image
}

void drawSprite(Sprite* sprite){//fonction permettant de dessiner un sprite
    C2D_SpriteSetPos(&sprite->sprite, sprite->x, sprite->y);//définit la position du sprite
    C2D_DrawSprite(&sprite->sprite);//on dessine maintenant le sprite dans le jeu
}