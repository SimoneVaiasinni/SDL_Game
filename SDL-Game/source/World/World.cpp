#include "World.h"
#include "../Utilities/BaseMinimal.h"
#include "../Entities/Entity.h"
#include "../Entities/Characters/Player.h"
#include "../Entities/Characters/Enemies/Skeleton.h"
#include "../Entities/Characters/Enemies/Slime.h"
#include "../Entities/FirePlace.h"
#include "../Game.h"

#include <ctime>
#include <typeinfo>

std::list<Entity*> World::Tiles = std::list<Entity*>();
std::list<FirePlace*> World::FirePlaces = std::list<FirePlace*>();
std::list<Character*> World::Characters = std::list<Character*>();

Entity* World::Ground = nullptr;

Entity* World::GetEntityByName(const char* name)
{
	for (auto c : Characters)
	{
		std::string TName = c->GetName();
		if (TName.compare(name) == 0)
			return c;
	}
	return nullptr;
}

void World::DrawText(Location* location, Dimension* dimension, Uint8 fontSize, std::string text)
{
	TTF_Font* Sans = TTF_OpenFont("./res/arial.ttf", fontSize);
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), SDL_Color{ 255,255,255 });
	SDL_Texture* Message = SDL_CreateTextureFromSurface(Game::Renderer, surfaceMessage);

	SDL_Rect* Message_rect = new SDL_Rect{ location->x, (location->y), dimension->w, dimension->h };
	SDL_RenderCopy(Game::Renderer, Message, NULL, Message_rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

void World::SpawnRandomEnemies()
{
	const int MAX_NUM_ENEMIES = 3;
	int nOfEnemies = rand() % MAX_NUM_ENEMIES;

	for (int i = 0; i < nOfEnemies; i++)
	{
		//Generate random X location
		int randomXLocation = rand() % 700 + 1;
		int xLocation = (Game::Screen_W - randomXLocation) + Game::Screen_W;

		Location* location = new Location{ xLocation, World::Ground->location.y - 64 };
		Dimension* dimension = new Dimension{ 64,64 };

		int typeOfEnemy = rand() % 100;
		//Spawn skeleton
		//---------------------------
		if (typeOfEnemy % 2 == 0)
		{
			auto skeleton = new Skeleton(*location, *dimension, "Skeleton");
			std::cout << "Skeleton at location: " << skeleton->location.ToString() << "\n";
		}
		//Spawn slime
		//-----------------------
		else
		{
			auto slime = new Slime(*location, *dimension, "Slime");
			std::cout << "slime at location: " << slime->location.ToString() << "\n";
		}
	}
	
}

void World::Update()
{
	for (auto C : Characters)
	{
		auto skeleton = dynamic_cast<Skeleton*>(C);
		auto slime = dynamic_cast<Slime*>(C);
		auto player = dynamic_cast<Player*>(C);

		if (skeleton)
			skeleton->Update();

		else if (player)
			player->Update();	

		else if (slime)
			slime->Update();		
	}
	
	for (auto F : FirePlaces)
		F->Update();
}

void World::Render()
{
	for (auto C : Characters)
	{
		bool inScreen = (C->location.x > 0 && C->location.x < Game::Screen_W);
		if (!inScreen) continue;

		auto skeleton = dynamic_cast<Skeleton*>(C);
		auto slime = dynamic_cast<Slime*>(C);
		auto player = dynamic_cast<Player*>(C);

		if (skeleton)
		{
			skeleton->Render();
			skeleton->DrawHealthBar(255,0,0,255, skeleton->Health);
		}
		else if (player)
		{
			player->Render();
			player->DrawHealthBar(0,255,0,255, player->Health);
		}	
		else if (slime)
		{
			slime->Render();
			slime->DrawHealthBar(255, 0, 0, 255, slime->Health);
		}
	}	

	for (auto F : FirePlaces)
	{
		bool inScreen = (F->location.x > 0 && F->location.x < Game::Screen_W);
		if (!inScreen) continue;
		else
			F->Render();
	}
}

bool World::Delay(double start, double delay)
{
	return (SDL_GetTicks() - start >= delay*1000);
}

int World::CountEnemyLevel()
{
	int count = 0;

	for (auto c : Characters)
	{
		auto skeleton = dynamic_cast<Skeleton*>(c);
		auto slime = dynamic_cast<Slime*>(c);
		if (skeleton)
		{
			if (!skeleton->IsDead() &&
				(skeleton->location.x > 0 && skeleton->location.x < Game::Screen_W))
			{
				count++;
			}
		}
		else if (slime)
		{
			if (!slime->IsDead() &&
				(slime->location.x > 0 && slime->location.x < Game::Screen_W))
			{
				count++;
			}
		}
	}

	return count;
}