#ifndef SPRITE_H
#define SPRITE_H
    #include <citro2d.h>//pour gestion du rendu 2D

    typedef struct{ //On crée une structure pour répresenter un Sprite
        C2D_Sprite sprite;//d'abord un sprite
        float x, y; //pour la gestion des déplacements suivant x et y
        float angle;//pour la gestion de la rotation par exemple
    }Sprite;

    void initializeSprite();//fonction permettant d'initialiser un sprite
#endif