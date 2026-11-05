#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;//notre structure jeu contenant toutes les structures dont a besoin pour créer et faire tourner le jeu
	initializeGame(&game);//on commence par initialiser le jeu
	runGameMainLoop(&game);//on lance la mainLoop du jeu
	exitGame(&game);//on quitte le jeu
	return 0;
}
