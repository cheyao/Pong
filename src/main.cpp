#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>

#include "game.hpp"

int main(int argv, char** args) {
        // Android needs argc and argv, but we don't use them, so...
        (void)argv;
        (void)args;

        // Rand
        srand(time(NULL));

        // Start our game!
        Game* game = new Game();

        if (game->initialize()) {
                // Failed to init
                return 1;
        }

	game->setBallCount(2);

        game->loop();

        game->close();

        return 0;
}

