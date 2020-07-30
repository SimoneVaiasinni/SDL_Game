#pragma once
#include "../Utilities/BaseMinimal.h"

class Entity;

class CollisionManager
{
public:
	CollisionManager() {}
	~CollisionManager() {}

	enum DirectionCollision {
		COLLISION_TOP = 0,
		COLLISION_BOTTOM = 1,
		COLLISION_LEFT = 2,
		COLLISION_RIGHT = 3
	};


	static bool SimpleCheckCollision(Entity* player, Entity* e2);
	static bool CheckCollision(Entity* player, unsigned int direction);

	static bool IsEntityOnTheGround(Entity* entity);
	static bool IsPlayerTouchedEnemy(Entity* player);

	static void PrintCollision(Entity* player);
};