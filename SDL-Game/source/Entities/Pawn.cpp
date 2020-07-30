#include "Pawn.h"

void Pawn::Move()
{
	switch (Direction)
	{
	case left:	location.x -= Speed;
		break;

	case right: location.x += Speed;
		break;

	case up: location.y -= Speed;
		break;

	case down: location.y += Speed;
		break;

	default:
		break;
	}
}