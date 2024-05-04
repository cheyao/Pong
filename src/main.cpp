#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>

#include "game.hpp"

int SDL_AppInit(void **appstate, int argc, char **argv) {
	(void)appstate;
	(void)argc;
	(void)argv;

	srand(time(NULL));

	Game *game = new Game();

	if (game->initialize()) {
		// Failed to init
		return -1;
	}

	game->addBall();

	*appstate = game;

	return 0;
}

int SDL_AppEvent(void *appstate, const SDL_Event *event) {
	return ((Game *)appstate)->handleEvent(*event);
}

int SDL_AppIterate(void *appstate) { return ((Game *)appstate)->loop(); }

void SDL_AppQuit(void *appstate) {
	((Game *)appstate)->close();
	delete((Game *)appstate);
}

