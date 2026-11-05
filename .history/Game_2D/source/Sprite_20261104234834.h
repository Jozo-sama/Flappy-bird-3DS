#ifndef SPRITE_H
#define SPRITE_H
    #include <citro2d.h>//pour gestion du rendu 2D

    typedef struct{ //On crée une structure pour répresenter un Sprite
        C2D_Sprite sprite;//d'abord un sprite
        float x, y; //pour la gestion des déplacements suivant x et y
        float angle;//pour la gestion de la rotation par exemple
    }Sprite;

    void loadSprite(Sprite* sprite, C2D_SpriteSheet spriteSheet, int spriteIndice);//fonction permettant de charger un sprite
    void drawSprite(Sprite* sprite);//fonction permettant de dessiner un sprite
    void setSpritePosition(Sprite* sprite, float x, float y);//fonction permettant de définit une nouvelle position du sprite
    void moveSprite(Sprite* sprite, float dx, float dy);//fonction permettant de déplacer un sprite

#endif