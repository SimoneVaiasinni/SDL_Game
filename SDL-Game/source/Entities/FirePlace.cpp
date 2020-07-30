#include "FirePlace.h"
#include "../TextureLoader.h"
#include "../World/World.h"
#include <list>
#include <time.h>

FirePlace::FirePlace(Location location, Dimension dimension, const char* name)
	:Pawn{location,dimension,name}
{
	Frames = rand() % 40;
	IsWeak = (Frames % 2 == 0);

	World::FirePlaces.push_back(this);
}

FirePlace::~FirePlace()
{}

void FirePlace::Update()
{
	DestRect->x = location.x;
	DestRect->y = location.y;
	DestRect->w = dimension.w;
	DestRect->h = dimension.h;

	Idle();
}

void FirePlace::Idle()
{	
	

	const int delay = 10; 
	Frames++;

	if (IsWeak)
	{
		if (Frames / delay > Weak.size() - 1)
			Frames = 0;

		Texture = TextureLoader::LoadTexture(Weak[Frames / delay].c_str());
	}
	else
	{
		if (Frames / delay > Strong.size() - 1)
			Frames = 0;

		Texture = TextureLoader::LoadTexture(Strong[Frames / delay].c_str());
	}
}