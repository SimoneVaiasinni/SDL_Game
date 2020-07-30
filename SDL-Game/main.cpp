#include "./source/Game.h"

#define SCREEN_WIDTH  1080
#define SCREEN_HEIGHT  720

#define FRAME_RATE 120
#define FRAME_DELAY (1000 / FRAME_RATE)

int main(int argc, char* args[])
{
	Game* __Game = new Game();
	__Game->Init(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", false);

	Uint32 FrameStart;
	int FrameTime;

	//Render loop
	while (__Game->IsRunning())
	{
		FrameStart = SDL_GetTicks();

		//Handle event
		__Game->HandleEvent();

		//Update
		__Game->Update();

		//Render
		__Game->Render();

		//Frame rate
		FrameTime = SDL_GetTicks() - FrameStart;

		if (FRAME_DELAY > FrameTime)
			SDL_Delay(FRAME_DELAY - FrameTime);
	}

	__Game->Destroy();

	return 0;
}