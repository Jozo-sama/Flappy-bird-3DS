#include "Game.h"
int main(int argc, char* argv[])
{
	initializeGame();//on commence par initialiser le jeu
	runGameMainLoop();//on lance la mainLoop du jeu
	exitGame();//on quitte le jeu
	return 0;
}
