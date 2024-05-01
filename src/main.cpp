#include <SDL2/SDL.h>

#include "game.hpp"
/*
#if defined(__CYGWIN__) || defined(ANDROID)
#include <SDL2/SDL_main.h>
int SDL_main() {
#else
*/
int main(int argv, char** args) {
	/*
#endif
*/
        Game* game = new Game();

        if (game->initialize()) {
                // Failed to init
                return 1;
        }

        game->loop();

        game->close();

	(void)argv;
	(void)args;

        return 0;
}

