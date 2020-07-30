#pragma once
#include "../Utilities/BaseMinimal.h"
#include <list>

class Entity;
class Character;
class Player;
class FirePlace;

class World {

public:
	World() {}

	static Entity* Ground;
	static std::list<Entity*> Tiles;
	static std::list<FirePlace*> FirePlaces;
	static std::list<Character*> Characters;

	static void Update();
	static void Render();
	static void DrawText(Location* location, Dimension* dimension, Uint8 fontSize, std::string text);

	static bool Delay(double start, double delay);
	static int CountEnemyLevel();
	static void SpawnRandomEnemies();

	static Entity* GetEntityByName(const char* name);

	template<class T>
	static Entity* GetSingleEntitieByClass();

	template<class T>
	static std::list<Entity*> GetEntitiesByClass();
};

template<class T>
inline std::list<Entity*> World::GetEntitiesByClass()
{
	std::list<Entity*> Res;
	for (auto c : Characters)
	{
		if (typeid(T) == typeid(*c))
			Res.push_back(c);
	}

	return Res;
}

template<class T>
inline Entity* World::GetSingleEntitieByClass()
{
	for (auto c : Characters)
	{
		if (typeid(T) == typeid(*c))
			return c;
	}
	return nullptr;
}