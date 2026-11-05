#ifndef TEXT_H
#define TEXT_H
    #include <citro2d.h>//pour gestion du rendu 2D

    typedef struct{//structure représentant une texture
        char texte[255];//le contenu du texte qui sera affiché. Par défaut on fixe la taille à 255
        C2D_Text texteC2D;
    }Text;

    void drawText(Text* text, C2D_TextBuf textBuffer, C2D_Font textFont);//permet d'afficher du texte dynamique à chaque frame
#endif