#include "Sprite.h"

void loadSprite(Sprite* sprite, C2D_SpriteSheet spriteSheet, int spriteIndice){//fonction permettant de charger un sprite
    C2D_SpriteFromSheet(&sprite->sprite, spriteSheet, spriteIndice);//on va charger la première texture
	C2D_SpriteSetCenter(&sprite->sprite, 0.0f, 0.0f);//le centre sera le point haut gauche de l'image
}

void drawSprite(Sprite* sprite){//fonction permettant de dessiner un sprite
    C2D_SpriteSetPos(&sprite->sprite, sprite->x, sprite->y);//définit la position du sprite
    C2D_DrawSprite(&sprite->sprite);//on dessine maintenant le sprite dans le jeu
}

void drawRotateSprite(Sprite* sprite){//fonction permettant de dessiner un sprite rotate
    C2D_SpriteSetPos(&sprite->sprite, sprite->x, sprite->y);//définit la position du sprite
    //
}

void setSpriteDimensions(Sprite* sprite, float w, float h){//définir les dimensions de l'image
    sprite->w = w;
    sprite->h = h;
}

void setSpritePosition(Sprite* sprite, float x, float y){//fonction permettant de définit une nouvelle position du sprite
    sprite->x = x;
    sprite->y = y;
}

void moveSprite(Sprite* sprite, float dx, float dy){//fonction permettant de déplacer un sprite
    sprite->x += dx;
    sprite->y += dy;
}

void setAngle(Sprite* sprite, float angle){
    sprite->angle = angle;
}

int collision_AABB(Sprite* box1, Sprite* box2){//retourne 0 si aucun collision, mais 1 si il y a collision entre des sprites
    if(box2->x >= box1->x + box1->w //box2 trop à droite
        || box2->x + box2->w <= box1->x //box 2 trop à gauche
        || box2->y >= box1->y + box1->h //box2 trop en bas
        || box2->y + box2->h <= box1->y){
            return 0;//on retourne false pas de collision
        }
    return 1;//par défaut on a une collision
}
int collision_AABB(Sprite* box1, Sprite* box2){//retourne 0 si aucun collision, mais 1 si il y a collision entre des sprites
    if(box2->x - box2->w / 2.0f >= box1->x - box1->w / 2.0f + box1->w //box2 trop à droite
        || box2->x - box2->w / 2.0f + box2->w <= box1->x - box1->w / 2.0f//box 2 trop à gauche
        || box2->y - box2->h / 2.0f>= box1->y - box1->h / 2.0f + box1->h //box2 trop en bas
        || box2->y - box2->h / 2.0f + box2->h <= box1->y - box1->h / 2.0f){
            return 0;//on retourne false pas de collision
        }
    return 1;//par défaut on a une collision
}