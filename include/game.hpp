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
	Uint32 mTicksCount;
#ifdef __ANDROID__
	Uint64 mLeftPressID;
	Uint64 mRightPressID;
	Vector2 mFingerL;
	Vector2 mFingerR;
#endif

	float THICKNESS;
	float PADDLE_HEIGHT;

	int input();
	int update();
	int draw();

	int updateBall(Ball& ball, float delta);

       public:
	Game();
	bool initialize();
	void setBallCount(int count);
	int loop();
	int handleEvent(SDL_Event event);
	void close();
};

#endif	// GAME_H
