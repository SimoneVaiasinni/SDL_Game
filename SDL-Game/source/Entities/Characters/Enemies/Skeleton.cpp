#include "Skeleton.h"
#include "../../../World/CollisionManager.h"
#include "../../../World/World.h"
#include "../../Characters/Player.h"
#include "../../../Game.h"
#include "../../../Utilities/SoundComponent.h"

Skeleton::Skeleton(Location location, Dimension dimension, const char* name)
	:Character{ location, dimension, name }
{
	LoadTextures(FilePath);
	Speed = 0.5;
}

Skeleton::~Skeleton()
{}

void Skeleton::Render()
{
	SDL_RenderCopy(Game::Renderer, Texture, NULL, DestRect);

	auto textDimension = new Dimension{ 20,20 };
	if (hit_react)
	{
		World::DrawText(
			new Location{ location.x + dimension.w / 2, location.y - 30 },
			textDimension,
			24,
			"-30"
		);
	}
	if (IsDead())
	{
		if (!World::Delay(TimeDeath, 1))
		{
			World::DrawText(
				new Location{ location.x + dimension.w / 2, location.y - 30 },
				textDimension,
				24,
				"-100"
			);
		}
	}
}

void Skeleton::Update()
{
	DestRect->x = location.x;
	DestRect->y = location.y;
	DestRect->w = dimension.w;
	DestRect->h = dimension.h;

	if (is_attacking && !IsHurt())
	{
		FramesHit = 0;
		FramesIdle = 0;

		const int Delay = 10;
		const int Size = (int)Images->Attack_R.size();
		FramesAttack++;

		auto player = World::GetSingleEntitieByClass<Player>();

		//Cause damage
		//---------------------
		if ((FramesAttack / Delay) == (Size / 2) - 2 &&
			CollisionManager::SimpleCheckCollision(this, player))
		{
			CauseDamage();
		}
		//Reset Frames
		//---------------------
		if ((FramesAttack / Delay) > (Size + 3))
		{
			FramesAttack = 0;
			is_attacking = false;
			DamageOnce = true;
		}
		
		//Set texture
		//---------------------
		if (Direction == left)
		{
			if ((FramesAttack / Delay) <= (Size - 1))
				Texture = Images->Attack_L[FramesAttack / Delay];
			else
				Texture = Images->Attack_L[(Size - 1)];
		}
		else
		{
			if ((FramesAttack / Delay) <= (Size - 1))
				Texture = Images->Attack_R[FramesAttack / Delay];
			else
				Texture = Images->Attack_R[Size - 1];
		}
			
	}
	if (hit_react)
	{
		FramesIdle = 0;
		FramesAttack = 0;

		const int Delay = 10;
		const int Size = (int)Images->HitReact_R.size();
		FramesHit++;

		//Reset Frames
		//---------------------
		if ((FramesHit / Delay) > (Size - 1))
		{
			FramesHit = 0;
			hit_react = false;
		}

		//Set texture
		//---------------------
		if (Direction == left)
			Texture = Images->HitReact_L[FramesHit / Delay];
		else
			Texture = Images->HitReact_R[FramesHit / Delay];
	}
	if (IsDead())
	{
		if (DeathOnce)
		{
			TimeDeath = SDL_GetTicks();
			DeathOnce = false;
		}

		hit_react = false;
		is_attacking = false;

		const int Delay = 10;
		const int Size = (int)Images->Death_L.size();
		
		if ((FramesDeath / Delay) <= (Size - 1))
			FramesDeath++;
		
		//Set texture
		//---------------------
		if (Direction == left)
		{
			if ((FramesDeath / Delay) <= (Size - 1))
				Texture = Images->Death_L[FramesDeath / Delay];
			else
				Texture = Images->Death_L[(Size - 1)];
		}
		else
		{
			if ((FramesDeath / Delay) <= (Size - 1))
				Texture = Images->Death_R[FramesDeath / Delay];
			else
				Texture = Images->Death_R[Size - 1];
		}
	}
}

void Skeleton::Attack()
{
	if (!is_attacking)
	{
		FramesIdle = 0;
		is_attacking = true;
	}
}

void Skeleton::CauseDamage()
{
	if (DamageOnce)
	{
		auto player = dynamic_cast<Player*>(World::GetSingleEntitieByClass<Player>());
		player->TakeDamage();
		DamageOnce = false;
	}
}

void Skeleton::Move()
{
	if (Direction == left)
		MoveToLeft();

	else
		MoveToRight();
}

void Skeleton::Idle()
{
	const int delay = 20;
	FramesIdle++;
	
	int Size = (int)Images->Idle_L.size();
	if (FramesIdle / delay > Size - 1)
		FramesIdle = 0;

	if (Direction == left)
		Texture = Images->Idle_L[FramesIdle / delay];
	else
		Texture = Images->Idle_R[FramesIdle / delay];
}

void Skeleton::MoveToLeft()
{
	const int Delay = 10;
	const int Size = (int) Images->Walk_L.size();
	FramesIdle++;
	(velocity <= 1 ? velocity += Speed : velocity = 0);
	location.x -= (int)velocity;
	
	if ((FramesIdle / Delay) > (Size - 1))
		FramesIdle = 0;

	Texture = Images->Walk_L[FramesIdle / Delay];
}

void Skeleton::MoveToRight()
{
	const int Delay = 10;
	const int Size = (int)Images->Walk_R.size();
	FramesIdle++;
	(velocity <= 1 ? velocity += Speed : velocity = 0);
	location.x += (int)velocity;

	if ((FramesIdle / Delay) > (Size - 1))
		FramesIdle = 0;

	Texture = Images->Walk_R[FramesIdle / Delay];
}

void Skeleton::TakeDamage()
{
	if (IsDead()) return;

	if (Health - 30 > 0)
		SoundComponent::PlayEffect(HIT_EFFECT);
	else
		SoundComponent::PlayEffect(DEATH_EFFECT);

	Health -= 30;
	FramesIdle = 0;
	hit_react = true;
}