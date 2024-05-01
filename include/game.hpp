#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL_video.h>
#pragma once

#include <SDL2/SDL.h>

typedef struct Vector2 {
        float x;
        float y;
} Vector2;

class Game {
      private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        int mWidth;
        int mHeight;
        Vector2 mBall;
        Vector2 mBallVelocity;
        Vector2 mPaddle;
        int mPaddleDir;
        bool mRunning;
        Uint32 mTicksCount;
        bool mMousePressed;

	int THICKNESS;
	int PADDLE_HEIGHT;

        void input();
        void update();
        void draw();

      public:
        Game();
        bool initialize();
        void loop();
        void close();
};

#endif // GAME_H
