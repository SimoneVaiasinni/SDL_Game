#pragma once
#include "../Utilities/BaseMinimal.h"

class Entity
{
public:
	Entity(Location location, Dimension dimension, const char* name);
	Entity(Location location, Dimension dimension, const char* name, SDL_Texture* Texture);
	~Entity();

	Location location;
	Dimension dimension;

	virtual void DrawRect();
	virtual void FillRect(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	virtual void Update();
	virtual void Render();

	const char* GetName() const { return name; }
	SDL_Texture* GetTexture() const { return Texture; }
	SDL_Rect* GetDestRect() const { return DestRect; }

protected:
	SDL_Texture* Texture = nullptr;
	SDL_Rect* DestRect = nullptr;
	const char* name = nullptr;
};