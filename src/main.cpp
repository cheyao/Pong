#include <SDL2/SDL.h>

#include "game.hpp"

int main() {
	Game* game = new Game();

	if (game->initialize()) {
		// Failed to init
		return 1;
	}

	game->loop();

	game->close();

	return 0;
}
