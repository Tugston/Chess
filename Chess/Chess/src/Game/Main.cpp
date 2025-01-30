#include "Game.h"
#include "Windows.h"
#include<span>
#define main SDL_main

//use graphics card, because my laptop has integrated graphics as well
#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) DWORD NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0;

#ifdef __cplusplus
}
#endif

int SDL_main(int argc, char *argv[]) {
	Chess::Game* ChessGame = new Chess::Game;
	ChessGame->Start();
	return 0;
}
//ENTRY POINT//