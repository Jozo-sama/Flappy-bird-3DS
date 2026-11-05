#include "Sprite.h"

void loadSprite(Sprite* sprite, C2D_SpriteSheet spriteSheet, int spriteIndice){//fonction permettant de charger un sprite
    C2D_SpriteFromSheet(&sprite->sprite, spriteSheet, spriteIndice);//on va charger la première texture
	C2D_SpriteSetCenter(&sprite->sprite, 0.0f, 0.0f);//le centre sera le point haut gauche de l'image
}

void drawSprite(Sprite* sprite){//fonction permettant de dessiner un sprite
    C2D_SpriteSetPos(&sprite->sprite, sprite->x, sprite->y);//définit la position du sprite
    C2D_DrawSprite(&sprite->sprite);//on dessine maintenant le sprite dans le jeu
}

void setSpritePosition(Sprite* sprite, float x, float y){//fonction permettant de définit une nouvelle position du sprite
    sprite->x = x;
    sprite->y = y;
}

void moveSprite(Sprite* sprite, float dx, float dy){//fonction permettant de déplacer un sprite
    sprite->x += dx;
    sprite->y += dy;
}