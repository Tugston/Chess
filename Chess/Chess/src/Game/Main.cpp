#include "Game.h"
#include<span>
#define main SDL_main
int SDL_main(int argc, char *argv[]) {
	Chess::Game* ChessGame = new Chess::Game;
	ChessGame->Start();
	return 0;
}
//ENTRY POINT//