#include "player.h"
#include "../../Game.h"
#include "../../World/CollisionManager.h"
#include "../../TextureLoader.h"
#include "../../World/World.h"
#include "../Characters/Enemies/Skeleton.h"
#include "../Characters/Enemies/Slime.h"
#include "../../Utilities/SoundComponent.h"

Player::Player(Location location, Dimension dimension, const char* name)
	: Character{ location, dimension ,name }
{
	LoadTextures(FilePath);
	Speed = 2;
}

Player::~Player()
{
	delete FilePath;
}

void Player::Update()
{
	DestRect->x = location.x;
	DestRect->y = location.y;
	DestRect->w = dimension.w;
	DestRect->h = dimension.h;

	if (location.x <= 0) location.x = 0;
	if (location.x >= Game::Screen_W - dimension.w) location.x = Game::Screen_W - dimension.w;
	if (location.y <= 0) location.y = 0;
	if (CollisionManager::IsEntityOnTheGround(this))
	{
		Falling = false;
		location.y = (World::Ground->location.y - 64) + 1;
	}

	if (!CollisionManager::IsEntityOnTheGround(this) &&
		!CollisionManager::CheckCollision(this, CollisionManager::COLLISION_BOTTOM) &&
		!IsInAir())
	{
		Gravity = 0;
		Falling = true;
	}

	//Death
	//----------
	if (IsDead())
	{
		const int Delay = 20;
		int Size = (int) Images->Death_R.size();
		if ((Frames / Delay) < Size - 1)
		{
			Frames++;
			Texture = Images->Death_R[(Frames / Delay)];
		}
	}

	//Jump
	//----------
	else if (Jumping)
	{
		Gravity -= 0.3;

		if (Gravity <= 0.f)
		{
			Jumping = false;
			Falling = true;
			Frames = 0;
		}
		else
		{
			location.y -= int(Gravity);

			Frames++;
			int Delay = 20;

			if (Direction == left)
			{
				int Size = (int) Images->Jump_L.size();
				if ((Frames / Delay) > Size - 1)
					Texture = Images->Jump_L[Size - 1];
				else
					Texture = Images->Jump_L[(Frames / Delay)];
			}
			else
			{
				int Size = (int)Images->Jump_L.size();
				if ((Frames / Delay) > Size - 1)
					Texture = Images->Jump_R[Size - 1];
				else
					Texture = Images->Jump_R[(Frames / Delay)];
			}

		}
	}
	else if (Falling)
	{
		Gravity += 0.3;
		location.y += int(Gravity);

		if (Direction == left)
			Texture = Images->Falling_L[0];
		else
			Texture = Images->Falling_R[0];

		if (CollisionManager::CheckCollision(this, CollisionManager::COLLISION_BOTTOM) ||
			CollisionManager::IsEntityOnTheGround(this))
		{
			Jumping = false;
			Falling = false;
		}
	}

	//Attack
	//----------
	else if (is_attacking)
	{
		Frames++;

		const int Delay = 7;
		int Size = (int) Images->Attack_L.size();
		if ((Frames / Delay) > (Size - 1))
		{
			Frames = 0;
			is_attacking = false;
			do_once = true;
		}

		//Cause damage
		//--------------
		if (CollisionManager::IsPlayerTouchedEnemy(this))
			CauseDamage();		

		//Set texture
		//---------------
		if (Direction == left)
			Texture = Images->Attack_L[Frames / Delay];
		else
			Texture = Images->Attack_R[Frames / Delay];
	}

	//Hurt
	//----------
	else if (is_hurt)
	{
		const int Delay = 20;
		Frames++;

		int Size = (int)Images->HitReact_L.size();
		if ((Frames / Delay) > (Size - 1))
		{
			Frames = 0;
			is_hurt = false;
		}

		if (Direction == left)
			Texture = Images->HitReact_L[Frames / Delay];
		else
			Texture = Images->HitReact_R[Frames / Delay];
	}
}

void Player::Move()
{
	if (is_attacking) return;
	if (is_hurt) return;

	if (Direction == EDirection::right)	MoveToRight();
	else MoveToLeft();
}

void Player::MoveToLeft()
{
	location.x -= Speed;

	if (!IsInAir()) Frames++;

	int Delay = 10;
	int Size = (int) Images->Run_L.size();
	if ((Frames / Delay) > (Size - 1))
		Frames = 0;

	Texture = Images->Run_L[Frames / Delay];
}

void Player::MoveToRight()
{
	location.x += Speed;

	if (!IsInAir()) Frames++;

	int Delay = 10;
	int size = (int)Images->Run_R.size();
	if ((Frames / Delay) > (size - 1))
		Frames = 0;

	Texture = Images->Run_R[Frames / Delay];
}

void Player::Idle()
{
	Frames++;
	int Size = (int)Images->Idle_L.size();
	if ((Frames / 30) > (Size - 1))
		Frames = 0;

	if (Direction == EDirection::left)
		Texture = Images->Idle_L[Frames / 30];
	else
		Texture = Images->Idle_R[Frames / 30];
}

void Player::Attack()
{
	//if (IsInAir()) return;

	is_attacking = true;
	Frames = 0;
}

void Player::Jump()
{
	Jumping = true;
	Gravity = 9.8;
	Texture = Images->Jump_R[0];
	Frames = 0;
}

void Player::SetDamageTimeOnce()
{
	if (do_once)
	{
		DamageTime = SDL_GetTicks();
		Health--;
		do_once = false;
	}
}

void Player::TakeDamage()
{
	if (Health > 0)
	{
		Health -= 30;

		if (Health < 0)
			Health = 0;

		is_hurt = true;
		Frames = 0;
	}
}

void Player::CauseDamage()
{
	if (!do_once) return;

	for (auto c : World::Characters)
	{
		auto skeleton = dynamic_cast<Skeleton*> (c);
		auto slime = dynamic_cast<Slime*> (c);
		if (skeleton)
		{
			if (CollisionManager::SimpleCheckCollision(this, skeleton))
			{
				skeleton->TakeDamage();
				do_once = false;
			}
		}
		else if (slime)
		{
			if (CollisionManager::SimpleCheckCollision(this, slime))
			{
				slime->TakeDamage();
				do_once = false;
			}
		}
	}
}

int Player::GetKills()
{
	int kills = 0;

	auto slimes = World::GetEntitiesByClass<Slime>();
	auto skeletons = World::GetEntitiesByClass<Skeleton>();

	for (auto sl : slimes)
	{
		auto castToSlime = dynamic_cast<Slime*>(sl);
		if (castToSlime->IsDead())
			kills++;
	}
	for (auto sk : skeletons)
	{
		auto castToSkeleton = dynamic_cast<Skeleton*>(sk);
		if (castToSkeleton->IsDead())
			kills++;
	}

	return kills;
}