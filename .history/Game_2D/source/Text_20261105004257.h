#ifndef TEXT_H
#define TEXT_H
    #include <citro2d.h>//pour gestion du rendu 2D

    typedef struct{//structure représentant une texture
        char texte[255];//le contenu du texte qui sera affiché. Par défaut on fixe la taille à 255
        C2D_Text texteC2D;//le C2D_texte
        float x, y;//la position de dessin du texte
        float scaleX, scaleY;//doit être comprise entre 0 et 1 et définit la taille 
        u32 textColor;//la couleur de dessin du texte
    }Text;

    void drawText(Text* text, C2D_TextBuf textBuffer, C2D_Font textFont, char message[]);//permet d'afficher du texte dynamique à chaque frame
    void setTextPosition(Text* text, float x, float y);//permet de spécifier la position de dessin du texte
    void setTextScale(Text* text, float scaleX, float scaleY);//permet de spécifier les dimensions du texte
    void setTextColor(Text* Text, u32 textColor);//permet de choisir la couleur de dessin du texte

#endif