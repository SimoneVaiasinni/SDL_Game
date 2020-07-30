#include "MathUtilities.h"
#include "../Entities/Entity.h"
#include <cmath>

int MathUtilities::DistanceBetweenEntities(Entity* e1, Entity* e2)
{
	int xa = e1->location.x + e1->dimension.w / 2;
	int xb = e2->location.x + e2->dimension.w / 2;

	int ya = e1->location.y + e1->dimension.h / 2;
	int yb = e2->location.y + e2->dimension.h / 2;

	int a = xa - xb; // Xa - Xb
	int b = ya - yb; // Ya - Yb
	int distance = int(sqrt(pow(a, 2) + pow(b, 2)));
	return distance;
}