#include <SDL2/SDL.h>

#include "game.hpp"

int main(int argv, char** args) {
	// Android needs argc and argv, but we don't use them, so...
	(void)argv;
	(void)args;

	// Start our game!
        Game* game = new Game();

        if (game->initialize()) {
                // Failed to init
                return 1;
        }

        game->loop();

        game->close();

        return 0;
}

