#include <SDL2/SDL.h>
#include <cstdio>

#include "game.hpp"

#define THICKNESS 15
#define PADDLE_HEIGHT 100

Game::Game()
    : mWindow(nullptr), mWidth(1024), mHeight(768), mBall({(float)mWidth / 2, (float)mHeight / 2}),
      mBallVelocity({-200.0f, 235.0f}), mPaddle({50, (float)mHeight / 2 - 20}), mPaddleDir(0),
      mRunning(true), mTicksCount(0) {}

void Game::input() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                        // Quitting the app
                        mRunning = false;
                        return;
                case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                                // Window was resized
                                mWidth = event.window.data1;
                                mHeight = event.window.data2;
                        }
                        break;
                }
        }

        // Process keyboard
        const Uint8* state = SDL_GetKeyboardState(NULL);

        // Default dir
        mPaddleDir = 0;
        if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) {
                // Pressed up
                mPaddleDir = -1;
        }

        if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) {
                // Pressed down
                mPaddleDir = 1;
        }
}

void Game::update() {
        // Wait for ticks
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
                ;
        float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

        // Max delta time
        if (deltaTime > 0.05f) {
                deltaTime = 0.05f;
        }

        // Update ticks count
        mTicksCount = SDL_GetTicks();

        if (mPaddleDir != 0) {
                mPaddle.y += mPaddleDir * 300.0f * deltaTime;
        }

        // Make sure paddle doesn't move off screen
        if (mPaddle.y < (PADDLE_HEIGHT / 2.0f + THICKNESS)) {
                mPaddle.y = PADDLE_HEIGHT / 2.0f + 100.0f;
        } else if (mPaddle.y > (mHeight - PADDLE_HEIGHT / 2.0f - THICKNESS)) {
                mPaddle.y = mHeight - PADDLE_HEIGHT / 2.0f - THICKNESS;
        }

        // Update ball
        mBall.x += mBallVelocity.x * deltaTime;
        mBall.y += mBallVelocity.y * deltaTime;

        float diff = mPaddle.y - mBall.y;
        // Normalize
        diff = (diff > 0.0f) ? diff : -diff;

        if (diff <= PADDLE_HEIGHT / 2.0f && mBall.x <= 70.0f && mBall.x >= 65.0f &&
            mBallVelocity.x < 0.0f) {
                // Paddle
                mBallVelocity.x *= -1.0f;
        } else if (mBall.x >= (mWidth - THICKNESS) && mBallVelocity.x > 0.0f) {
                // Right wall
                mBallVelocity.x *= -1.0f;
        } else if (mBall.x <= 0.0f && mBallVelocity.x < 0.0f) {
                // Off screen
                mRunning = false;
        }

        if (mBall.y <= 15 && mBallVelocity.y < 0.0f) {
                // Top wall
                mBallVelocity.y *= -1;
        } else if (mBall.y >= (mHeight - 15) && mBallVelocity.y > 0.0f) {
                // Bottom wall
                mBallVelocity.y *= -1;
        }
}

void Game::draw() {
        // Set draw color to blue
        SDL_SetRenderDrawColor(mRenderer,
                               0,   // R
                               0,   // G
                               255, // B
                               255  // A
        );

        // Clear back buffer
        SDL_RenderClear(mRenderer);

        // Draw walls
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

        // Draw top wall
        SDL_Rect wall{
                0,        // Top left x
                0,        // Top left y
                mWidth,   // Width
                THICKNESS // Height
        };
        SDL_RenderFillRect(mRenderer, &wall);

        // Draw bottom wall
        wall.y = mHeight - 15;
        SDL_RenderFillRect(mRenderer, &wall);

        // Draw right wall
        wall.x = mWidth - THICKNESS;
        wall.y = 0;
        wall.w = THICKNESS;
        wall.h = mHeight;
        SDL_RenderFillRect(mRenderer, &wall);

        // Draw paddle
        SDL_Rect paddle{static_cast<int>(mPaddle.x),
                        static_cast<int>(mPaddle.y - PADDLE_HEIGHT / 2.0f), THICKNESS,
                        static_cast<int>(PADDLE_HEIGHT)};
        SDL_RenderFillRect(mRenderer, &paddle);

        // Draw ball
        SDL_Rect ball{static_cast<int>(mBall.x - THICKNESS / 2.0f),
                      static_cast<int>(mBall.y - THICKNESS / 2.0f), THICKNESS, THICKNESS};
        SDL_RenderFillRect(mRenderer, &ball);

        // Swap front buffer and back buffer
        SDL_RenderPresent(mRenderer);
}

void Game::loop() {
        while (mRunning) {
                input();
                update();
                draw();
        }
}

bool Game::initialize() {
        if (SDL_Init(SDL_INIT_VIDEO)) {
                SDL_Log("Unable to init SDL: %s\n", SDL_GetError());
                return 1;
        }

        mWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth,
                                   mHeight, SDL_WINDOW_RESIZABLE);
        if (mWindow == NULL) {
                SDL_Log("Failed to create window: %s", SDL_GetError());
                return 1;
        }

        mRenderer = SDL_CreateRenderer(mWindow, -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (mRenderer == NULL) {
                SDL_Log("Failed to create renderer: %s", SDL_GetError());
        }

        return 0; // Succesfully initialized!
}

void Game::close() {
        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
}

