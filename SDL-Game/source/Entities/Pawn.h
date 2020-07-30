#pragma once
#include "Entity.h"

enum EDirection { right = 0, left = 1, up = 2, down = 3 };

class Pawn : public Entity
{

public:
	Pawn(Location location, Dimension dimension, const char* name, SDL_Texture* Texture)
		: Entity{ location , dimension ,name , Texture } {}

	Pawn(Location location, Dimension dimension, const char* name)
		: Entity{ location , dimension ,name } {}

	~Pawn() {}

	EDirection Direction = right;

	double Speed = 1;
	virtual void Idle() {};
	virtual void Move();

	void DecrementX() { location.x--; }
	void DecrementY() { location.y--; }
	void IncreaseX() { location.x++; }
	void IncreaseY() { location.y++; }
};