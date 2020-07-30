#include "AIComponent.h"
#include "CollisionManager.h"
#include "World.h"
#include "../Utilities/MathUtilities.h"
#include "../Entities/Characters/Player.h"
#include "../Entities/Characters/Enemies/Skeleton.h"
#include "../Entities/Characters/Enemies/Slime.h"
#include "../Game.h"

void AIComponent::Possess(std::list<Character*> characters)
{
	Characters = characters;
}

void AIComponent::Control()
{
	if (Characters.empty()) return;

	for (auto c : Characters)
	{
		auto player = dynamic_cast<Player*>(World::GetSingleEntitieByClass<Player>());
		bool inScreen = (c->location.x > 0 && c->location.x < Game::Screen_W);
		if (!inScreen || c == player) continue;

		auto skeleton = dynamic_cast<Skeleton*> (c);
		auto slime = dynamic_cast<Slime*> (c);

		if (skeleton)
		{
			//If enemy is alive
			//---------------
			if (!skeleton->IsDead())
			{
				//Follow player
				//-------------------
				if (!CollisionManager::SimpleCheckCollision(player, skeleton))
				{
					//Set direction
					if (player->location.x < skeleton->location.x)
						skeleton->Direction = EDirection::left;
					else
						skeleton->Direction = EDirection::right;

					//Move - idle
					
					if (inScreen && !skeleton->IsHurt() && !skeleton->IsAttacking())
						skeleton->Move();

					else if (Game::TranslateMap)
						skeleton->Idle();
				}
				//Attack 
				//-------------------
				else
				{
					if (!skeleton->IsAttacking() && !skeleton->IsHurt() && !player->IsInAir())
						skeleton->Attack();
				}
			}
		}
		else if (slime)
		{
			//If enemy is alive
			//---------------
			if (!slime->IsDead())
			{
				//Follow player
				//-------------------
				if (!CollisionManager::SimpleCheckCollision(player, slime))
				{
					//Set direction
					if (player->location.x < slime->location.x)
						slime->Direction = EDirection::left;
					else
						slime->Direction = EDirection::right;

					//Move - idle
					if (inScreen && !slime->IsHurt() && !slime->IsAttacking())
						slime->Move();

					else if (Game::TranslateMap)
						slime->Idle();
				}
				//Attack 
				//-------------------
				else
				{
					if (!slime->IsAttacking() && !slime->IsHurt() && !player->IsInAir())
						slime->Attack();
				}
			}
		}
	}
}