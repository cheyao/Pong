#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <cstdio>

#include "game.hpp"

Game::Game()
    : mWindow(nullptr), mWidth(1024), mHeight(768), mBallVelocity({-200.0f, 235.0f}),
      mPaddleADir(0), mPaddleBDir(0), mRunning(true), mTicksCount(0), THICKNESS(15),
      PADDLE_HEIGHT(100) {}

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
                                mPaddleB.x = mWidth - 75;
                        }
                        break;

#ifdef __ANDROID__
                case SDL_FINGERDOWN:
                        if (event.tfinger.x * mWidth < mWidth / 2) {
                                mLeftPressID = event.tfinger.fingerId;
                        } else {
                                mRightPressID = event.tfinger.fingerId;
                        }
                        // Handle the rest of press with motion
                case SDL_FINGERMOTION:
                        // X and Y are from 0 to 1!?!?
                        if (mLeftPressID == event.tfinger.fingerId) {
                                mFingerL.x = event.tfinger.x * mWidth;
                                mFingerL.y = event.tfinger.y * mHeight;
                        } else if (mRightPressID == event.tfinger.fingerId) {
                                mFingerR.x = event.tfinger.x * mWidth;
                                mFingerR.y = event.tfinger.y * mHeight;
                        }
                        break;

                case SDL_FINGERUP:
                        if (event.tfinger.fingerId == mLeftPressID) {
                                mLeftPressID = -1; // No press
                                mPaddleADir = 0;
                                mFingerL.x = -1;
                                mFingerL.y = -1;
                        } else if (event.tfinger.fingerId == mRightPressID) {
                                mRightPressID = -1;
                                mPaddleBDir = 0;
                                mFingerR.x = -1;
                                mFingerR.y = -1;
                        }
                        break;
#endif

                default:
                        // Don't handle, we don't need this event
                        break;
                }
        }

#ifdef __ANDROID__
        if (mFingerL.x != -1) {
                if (mFingerL.y < mPaddleA.y + 25) { // higher than paddle
                        mPaddleADir = -1;
                } else if (mFingerL.y > mPaddleA.y + PADDLE_HEIGHT - 25) { // lower
                        mPaddleADir = 1;
                } else {
			mPaddleADir = 0;
		}
        }

        if (mFingerR.x != -1) {
                if (mFingerR.y < mPaddleB.y + 25) { // higher then paddle
                        mPaddleBDir = -1;
                } else if (mFingerR.y > mPaddleB.y + PADDLE_HEIGHT -25) { // lower
                        mPaddleBDir = 1;
                } else {
			mPaddleBDir = 0;
		}
        }
#else
        // Process keyboard
        const Uint8* state = SDL_GetKeyboardState(NULL);

        // Paddle A
        mPaddleADir = 0;
        if (state[SDL_SCANCODE_W]) {
                // Pressed up
                mPaddleADir = -1;
        }

        if (state[SDL_SCANCODE_S]) {
                // Pressed down
                mPaddleADir = 1;
        }

        // Paddle B
        mPaddleBDir = 0;
        if (state[SDL_SCANCODE_I]) {
                mPaddleBDir = -1;
        }

        if (state[SDL_SCANCODE_K]) {
                mPaddleBDir = 1;
        }
#endif
}

void Game::update() {
        // Wait for 16 ticks to pass
        while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
                ;
        float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

        // Max delta time
        if (deltaTime > 0.05f) {
                deltaTime = 0.05f;
        }

        // Update ticks count
        mTicksCount = SDL_GetTicks();

        // Update Movements
        if (mPaddleADir != 0) {
                mPaddleA.y += mPaddleADir * 300.0f * deltaTime;
        }
        if (mPaddleBDir != 0) {
                mPaddleB.y += mPaddleBDir * 300.0f * deltaTime;
        }

        // Make sure paddle doesn't move off screen
        if (mPaddleA.y < (PADDLE_HEIGHT / 2.0f + THICKNESS)) {
                mPaddleA.y = PADDLE_HEIGHT / 2.0f + THICKNESS;
        } else if (mPaddleA.y > (mHeight - PADDLE_HEIGHT / 2.0f - THICKNESS)) {
                mPaddleA.y = mHeight - PADDLE_HEIGHT / 2.0f - THICKNESS;
        }
        if (mPaddleB.y < (PADDLE_HEIGHT / 2.0f + THICKNESS)) {
                mPaddleB.y = PADDLE_HEIGHT / 2.0f + THICKNESS;
        } else if (mPaddleB.y > (mHeight - PADDLE_HEIGHT / 2.0f - THICKNESS)) {
                mPaddleB.y = mHeight - PADDLE_HEIGHT / 2.0f - THICKNESS;
        }

        // Update ball
        mBall.x += mBallVelocity.x * deltaTime;
        mBall.y += mBallVelocity.y * deltaTime;

        // Check if ball hit paddleA
        float diffA = mPaddleA.y - mBall.y;
        diffA = (diffA > 0.0f) ? diffA : -diffA;

        if (diffA <= PADDLE_HEIGHT / 2.0f && mBall.x <= 70.0f && mBall.x >= 65.0f &&
            mBallVelocity.x < 0.0f) {
                mBallVelocity.x *= -1.0f;
        }

        // Check if ball hit paddleB
        float diffB = mPaddleB.y - mBall.y;
        diffB = (diffB > 0.0f) ? diffB : -diffB;

        if (diffB <= PADDLE_HEIGHT / 2.0f && mBall.x >= mWidth - 80.0f &&
            mBall.x <= mWidth - 75.0f && mBallVelocity.x > 0.0f) {
                mBallVelocity.x *= -1.0f;
        }

        // Check if ball hit top or bottom wall
        if (mBall.y <= 15 && mBallVelocity.y < 0.0f) { // Top
                mBallVelocity.y *= -1;
        } else if (mBall.y >= (mHeight - 15) && mBallVelocity.y > 0.0f) { // Bottom
                mBallVelocity.y *= -1;
        }

        // Check if off screen
        if ((mBall.x < 0 && mBallVelocity.x < 0.0f) ||      // Left Wall
            (mBall.x > mWidth && mBallVelocity.x > 0.0f)) { // Right wall
                mRunning = false;
        }
}

void Game::draw() {
        // Clear background to blue
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
        SDL_RenderClear(mRenderer);

        // Draw top and bottom wall
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

        SDL_Rect wall{0, 0, mWidth, THICKNESS};
        SDL_RenderFillRect(mRenderer, &wall);

        wall.y = mHeight - THICKNESS;
        SDL_RenderFillRect(mRenderer, &wall);

        // Draw paddle
        SDL_Rect paddle{static_cast<int>(mPaddleA.x),
                        static_cast<int>(mPaddleA.y - PADDLE_HEIGHT / 2.0f),
                        static_cast<int>(THICKNESS), static_cast<int>(PADDLE_HEIGHT)};
        SDL_RenderFillRect(mRenderer, &paddle);

        paddle.x = static_cast<int>(mPaddleB.x);
        paddle.y = static_cast<int>(mPaddleB.y - PADDLE_HEIGHT / 2.0f);

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

        // Reset bonds, mostly for Android
        SDL_GetWindowSize(mWindow, &mWidth, &mHeight);
        mBall = {(float)mWidth / 2, (float)mHeight / 2};
        mPaddleA = {50, (float)mHeight / 2 - (float)PADDLE_HEIGHT / 2};
        mPaddleB = {(float)mWidth - 75, (float)mHeight / 2 - (float)PADDLE_HEIGHT / 2};

        mRenderer = SDL_CreateRenderer(mWindow, -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (mRenderer == NULL) {
                SDL_Log("Failed to create renderer: %s", SDL_GetError());
        }

#ifdef __ANDROID__
        mFingerL.x = mFingerL.y = mFingerR.x = mFingerR.y = -1;
#endif

        return 0; // Succesfully initialized!
}

void Game::close() {
        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
}

