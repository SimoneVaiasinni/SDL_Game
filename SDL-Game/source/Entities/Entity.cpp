#include "Entity.h"
#include "../Game.h"
#include "../World/World.h"
#include "../World/CollisionManager.h"

Entity::Entity(Location location, Dimension dimension, const char* name)
	: location{ location }, dimension{ dimension }, name{ name }
{
	DestRect = new SDL_Rect();
	DestRect->h = dimension.h;
	DestRect->w = dimension.w;
	DestRect->x = location.x;
	DestRect->y = location.y;
}

Entity::Entity(Location location, Dimension dimension, const char* name, SDL_Texture* Texture)
	:location{ location }, dimension{ dimension }, name{ name }, Texture{ Texture }
{
	DestRect = new SDL_Rect();
	DestRect->h = dimension.h;
	DestRect->w = dimension.w;
	DestRect->x = location.x;
	DestRect->y = location.y;
}

Entity::~Entity()
{
	delete DestRect;
	delete name;
}

void Entity::Update()
{
	DestRect->h = dimension.h;
	DestRect->w = dimension.w;
	DestRect->x = location.x;
	DestRect->y = location.y;
}

void Entity::Render()
{
	if (Texture)
		SDL_RenderCopy(Game::Renderer, Texture, NULL, DestRect);
	else
		SDL_RenderCopy(Game::Renderer, NULL, NULL, DestRect);
}

void Entity::DrawRect()
{
	SDL_SetRenderDrawColor(Game::Renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(Game::Renderer, DestRect);
	SDL_SetRenderDrawColor(Game::Renderer, 0, 0, 0, 255);
}

void Entity::FillRect(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(Game::Renderer, r, g, b, a);
	SDL_RenderFillRect(Game::Renderer, DestRect);
	SDL_SetRenderDrawColor(Game::Renderer, 0, 0, 0, 255);
}