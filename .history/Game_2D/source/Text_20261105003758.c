#include "Text.h"

void drawText(Text* text, C2D_TextBuf textBuffer, C2D_Font textFont, char message[]){//permet d'afficher du texte dynamique à chaque frame
    snprintf(text->texte, sizeof(text->texte), message);//on va copier le contenu du texte à afficher dynamiquement 
    C2D_TextFontParse(&text->texteC2D, textFont, textBuffer, text->texte);//on transfère maintenant le contenu de texte dans texteC2D
    C2D_TextOptimize(&text->texteC2D);//on optimise le texte pour son affichage
}

void setTextPosition(Text* text, float x, float y);//permet de spécifier la position de dessin du texte