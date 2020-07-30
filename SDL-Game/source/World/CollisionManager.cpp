#include "CollisionManager.h"
#include "World.h"
#include "../Entities/Characters/Player.h"
#include "../Entities/Characters/Enemies/Skeleton.h"
#include "../Entities/Characters/Enemies/Slime.h"

bool CollisionManager::SimpleCheckCollision(Entity* player, Entity* e2)
{
	return (SDL_HasIntersection(player->GetDestRect(), e2->GetDestRect()));
}

bool CollisionManager::CheckCollision(Entity* player, unsigned int direction)
{
	for (auto e : World::Tiles)
	{
		if (SimpleCheckCollision(player, e) && e != World::Ground)
		{
			if (direction == COLLISION_TOP)
			{
				if (player->location.y > e->location.y)	return true;
			}
			else if (direction == COLLISION_BOTTOM)
			{
				if (player->location.y + player->dimension.h >= e->location.y)
				{
					player->location.y = e->location.y - (e->dimension.h - 1);
					return true;
				}
			}
			else if (direction == COLLISION_LEFT)
			{
				SDL_Rect* Velocity = new SDL_Rect{
					player->location.x,
					player->location.y + (player->dimension.h / 2),
					player->dimension.w / 2,
					10 };
				if (SDL_HasIntersection(Velocity, e->GetDestRect()))	return true;
			}
			else if (direction == COLLISION_RIGHT)
			{
				SDL_Rect* Velocity = new SDL_Rect{
					player->location.x + (player->dimension.w / 2),
					player->location.y + (player->dimension.h / 2),
					player->dimension.w / 2,
					10 };
				if (SDL_HasIntersection(Velocity, e->GetDestRect()))	return true;
			}
		}
	}
	return false;
}

bool CollisionManager::IsEntityOnTheGround(Entity* entity)
{
	return (SimpleCheckCollision(entity, World::Ground));
}

bool CollisionManager::IsPlayerTouchedEnemy(Entity* player)
{
	auto skeletons = World::GetEntitiesByClass<Skeleton>();
	auto slimes = World::GetEntitiesByClass<Slime>();

	for (auto s : skeletons)
	{
		if (SimpleCheckCollision(player, s))
			return true;
	}
	for (auto s : slimes)
	{
		if (SimpleCheckCollision(player, s))
			return true;
	}
	return false;
}

void CollisionManager::PrintCollision(Entity* player)
{
	for (auto& e : World::Tiles)
	{
		if (SDL_HasIntersection(player->GetDestRect(), e->GetDestRect()) &&
			e != World::Ground)

			std::cout << "Collision " << player->GetName() << " - " << e->GetName() << std::endl;
	}
}