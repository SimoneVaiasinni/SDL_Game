#include "Game.h"
#include "TextureLoader.h"
#include "./World/CollisionManager.h"
#include "./World/World.h"
#include "./Entities/Entity.h"
#include "./Entities/Characters/Player.h"
#include "./Entities/Characters/Enemies/Skeleton.h"
#include "./Entities/Characters/Enemies/Slime.h"
#include "./Entities/FirePlace.h"
#include "./Utilities/SoundComponent.h"

#include "Utilities/MathUtilities.h"
#include "World/AIComponent.h"

SDL_Renderer* Game::Renderer = nullptr;
SDL_Window* Game::Window = nullptr;

int Game::Screen_W = 0;
int Game::Screen_H = 0;
int Game::Level = 0;
bool Game::TranslateMap = false;

void Game::Init(int w, int h, const char* title, bool fullscreen)
{
	Game::Screen_W = w;
	Game::Screen_H = h;

	//Init SDL
	//------------
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) PrintError("Initialize sdl2 failed: ", SDL_GetError());
	else
	{
		TTF_Init();

		//Create window
		//------------
		Window = SDL_CreateWindow(title, 100, 100, w, h, (fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN));
		if (!Window) PrintError("Create window failed: ", SDL_GetError());
		else
		{
			//Create Renderer
			//------------
			Renderer = SDL_CreateRenderer(Window, -1, 0);
			if (Renderer)
			{
				SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
				is_running = true;

				//Create scene
				//---------------------------------------------------------------

				//Background
				auto backgroundTxt = TextureLoader::LoadTexture("./res/img/Forest.png");
				Background1 = new Entity(Location(0,0), Dimension(w,h), "Background", backgroundTxt);
				Background2 = new Entity(Location(w, 0), Dimension(w, h), "Background", backgroundTxt);
				
				//Trigger scrol map
				TriggerScrollingMap = new Entity(Location(w - 20, -20), Dimension(20, h + 20), "Trigger");
				World::Tiles.push_back(TriggerScrollingMap);

				//Ground ----------
				World::Ground = new Entity(Location(0, h - 60), Dimension(w, 60), "Ground");

				//Adding player ----------
				__Player = new Player(Location(100, World::Ground->location.y - 64), Dimension(64, 64), "Player");

				//Adding Enemies ------------
				World::SpawnRandomEnemies();

				//Adding Fires ------------
				auto fire = new FirePlace(Location(400, World::Ground->location.y - 64), Dimension(64, 64), "FirePlace_1");
				fire = new FirePlace(Location(1000, World::Ground->location.y - 64), Dimension(64, 64), "FirePlace_2");

				//Create AI
				//---------------------------------------------------------------
				AIcomponent = new AIComponent();
				
				//Play music
				//----------------------
				SoundComponent::PlayMusic("./res/sound/Music_2.mp3");
			}
		}
	}
}

void Game::HandleEvent()
{
	SDL_Event WindowEvent;
	SDL_PollEvent(&WindowEvent);

	if (WindowEvent.type == SDL_QUIT) is_running = false;

	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	bool IsStopped = (!keystates[SDL_SCANCODE_A] &&
		!keystates[SDL_SCANCODE_D] &&
		!__Player->IsAttacking() &&
		!__Player->IsInAir()
		);

	//Is stopped
	if (IsStopped)
	{
		if (__Player->IsDead()) return;
			__Player->Idle();
	}

	//A (Left)
	else if (keystates[SDL_SCANCODE_A])
	{
		if (__Player->IsDead()) return;
		if (__Player->IsAttacking()) return;
		if (TranslateMap) return;

		__Player->Direction = EDirection::left;

		bool CollisionLeft = CollisionManager::CheckCollision(__Player, CollisionManager::COLLISION_LEFT);

		if (CollisionLeft)
			__Player->Idle();
		
		else
			__Player->Move();
	}

	//D (Right)
	else if (keystates[SDL_SCANCODE_D])
	{
		if (__Player->IsDead()) return;
		if (__Player->IsAttacking()) return;

		__Player->Direction = EDirection::right;
		bool CollisionRight = CollisionManager::CheckCollision(__Player, CollisionManager::COLLISION_RIGHT);

		if (CollisionRight)
			__Player->Idle();
		else
			__Player->Move();
	}

	//W (Up)
	else if (keystates[SDL_SCANCODE_W])
	{}

	//S (Down)
	else if (keystates[SDL_SCANCODE_S])
	{}

	//Spacebar
	if (keystates[SDL_SCANCODE_SPACE])
	{
		bool CollisionRight = CollisionManager::CheckCollision(__Player, CollisionManager::COLLISION_RIGHT);

		if (__Player->IsAttacking()) return;
		if (TranslateMap) return;
		if (CollisionRight) return;

		if (!SpacebarPressedOnce)
		{
			if (!__Player->IsInAir())
			{
				__Player->Jump();
				SpacebarPressedOnce = true;
			}
		}
	}
	else
		SpacebarPressedOnce = false;

	//Left mouse button
	bool isLMBClicked = (WindowEvent.type == SDL_MOUSEBUTTONUP && WindowEvent.button.button == SDL_BUTTON_LEFT);
	if (isLMBClicked)
	{
		if (!__Player->IsAttacking() && !__Player->IsHurt())
			__Player->Attack();
	}
}

void Game::Update()
{
	if (SpawnEnemiesOnce)
	{
		World::SpawnRandomEnemies();
		SpawnEnemiesOnce = false;
	}
	
	//Update all entities
	World::Update();

	//Update background
	Background1->Update();
	Background2->Update();

	//Translate map
	if (TranslateMap)
		Translate();
	
	//Player overlaps Trigger
	//Enemies left = 0
	if (CollisionManager::SimpleCheckCollision(__Player, TriggerScrollingMap) &&
		World::CountEnemyLevel() == 0)
	{
		TranslateMap = true;
		SpawnEnemiesOnce = true;
	}
		
	else if (CollisionManager::SimpleCheckCollision(__Player, TriggerScrollingMap) &&
		World::CountEnemyLevel() > 0)
		PrintLog = true;
	
	else if (!CollisionManager::SimpleCheckCollision(__Player, TriggerScrollingMap))
		PrintLog = false;
	
	//Switch background
	if (Background1->location.x + Screen_W <= 0)
	{
		Background1->location.x = 0;
		Background2->location.x = Screen_W;
	}
	
	//AI Control
	AIcomponent->Possess(World::Characters);
	AIcomponent->Control();

	if (!__Player->IsDead())
		TimeAlive = SDL_GetTicks() / 1000;
}

void Game::Render()
{
	SDL_RenderClear(Renderer);

	//Background
	Background1->Render();
	Background2->Render();

	//Render all entities
	World::Render();

	//Draw Level text
	//------------------------
	{
		Location* textLocation = new Location{Screen_W/2, 50};
		Dimension* textDimension = new Dimension{60,30};
		auto string = "Level: " + std::to_string(Level);
		World::DrawText(textLocation, textDimension, 24, string.c_str());
	}
	//------------------------


	//Player death
	//Draw text
	//-----------------------
	if (__Player->IsDead())
	{
		Location* textLocation = new Location{ __Player->location.x,(__Player->location.y - 10) - posText};
		Dimension* textDimension = new Dimension{100,20};
		World::DrawText(textLocation, textDimension, 24, "You are dead!");
		if (posText <= 100)	posText++;

		//Draw GUI
		//-------------------------
		auto GUI = new Entity(Location(Screen_W / 2 - 250, 200), Dimension(500, 200), "");
		GUI->FillRect(114, 160, 193, 0);

		//Level Text
		auto stringLevel = "Level: " + std::to_string(Level);
		World::DrawText(new Location(GUI->location.x + 50, GUI->location.y + 20), new Dimension(60, 20), 24, stringLevel.c_str());

		//Kills text
		int Kills = __Player->GetKills();
		auto stringKills = "Kills: " + std::to_string(Kills);
		World::DrawText(new Location(GUI->location.x + 50, GUI->location.y + 50), new Dimension(60, 20), 24, stringKills.c_str());

		//Time alive text
		auto stringTime = "Time alive: " + std::to_string(TimeAlive) + " seconds";
		World::DrawText(new Location(GUI->location.x + 50, GUI->location.y + 80), new Dimension(120, 30), 24, stringTime.c_str());
		
		World::DrawText(new Location(GUI->location.x + 200, GUI->location.y + 130), new Dimension(120, 30), 30, "YOU ARE DEAD");
	}
	else if (PrintLog)
	{
		Location* textLocation = new Location{__Player->location.x - 150, __Player->location.y - 50};
		Dimension* textDimension = new Dimension{ 200,20 };
		World::DrawText(textLocation, textDimension, 20, "You must kill all enemies!");
	}
	//-----------------------

	SDL_RenderPresent(Renderer);
}

void Game::Destroy()
{
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Game::Translate()
{
	Background1->location.x -= 5;
	Background2->location.x -= 5;

	for (auto c : World::Characters)
		c->location.x -= OffsetX;
	for (auto f : World::FirePlaces)
		f->location.x -= OffsetX;

	if (__Player->location.x <= OffsetX)
	{
		TranslateMap = false;
		Level++;
	}
}