#ifndef GAME_H
#define GAME_H
#pragma once

#include <SDL3/SDL.h>
#include <vector>

typedef struct Vector2 {
        float x;
        float y;
} Vector2;

typedef struct Ball {
        Vector2 position;
        Vector2 velocity;
} Ball;

class Game {
      private:
        SDL_Window* mWindow;
        SDL_Renderer* mRenderer;
        int mWidth;
        int mHeight;
        std::vector<Ball> mBalls;
        Vector2 mPaddleA;
        int mPaddleADir;
        Vector2 mPaddleB;
        int mPaddleBDir;
        bool mRunning;
        Uint32 mTicksCount;
#ifdef __ANDROID__
        int mLeftPressID;
        int mRightPressID;
        Vector2 mFingerL;
        Vector2 mFingerR;
#endif

        float THICKNESS;
        float PADDLE_HEIGHT;

        void input();
        void update();
        void draw();
        void updateBall(Ball& ball, float delta);

      public:
        Game();
        bool initialize();
        void setBallCount(int count);
        void loop();
        void close();
};

#endif // GAME_H
