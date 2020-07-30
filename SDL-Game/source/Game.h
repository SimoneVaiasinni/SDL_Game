#pragma once
#include "Utilities/BaseMinimal.h"

class Game
{
public:
	Game() {}

	void Init(int w, int h, const char* title, bool fullscreen);
	bool IsRunning() const { return is_running; }
	void HandleEvent();
	void Update();
	void Render();
	void Destroy();

	void Translate();

	static SDL_Renderer* Renderer;
	static SDL_Window* Window;
	
	static int Screen_W;
	static int Screen_H;
	static int Level;
	static bool TranslateMap;

private:
	class Player* __Player = nullptr;
	class AIComponent* AIcomponent = nullptr;

	class Entity* Background1 = nullptr;
	class Entity* Background2 = nullptr;
	class Entity* TriggerScrollingMap = nullptr;
	
	bool is_running = false;
	bool SpacebarPressedOnce = false;

	bool PrintLog = false;
	bool SpawnEnemiesOnce = false;

	int posText = 0;
	int OffsetX = 5;

	Uint32 TimeAlive = 0;

	//Print Log debug
	//--------------------------------
	template <typename T>
	void PrintError(const char* Text, T Val) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "title", Text, Window); }

	template <typename T>
	void Print(const char* Text, T Val) { std::cout << Text << Val << std::endl; }
	void Print(const char* Text) { std::cout << Text << std::endl; }
};