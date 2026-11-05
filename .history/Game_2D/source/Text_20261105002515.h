#ifndef TEXT_H
#define TEXT_H
    #include <citro2d.h>//pour gestion du rendu 2D
    typedef struct{//structure représentant une texture
        char texte[255];//le texte qui sera affiché.
        C2D_Text texteC2D;
    }Text;
#endif