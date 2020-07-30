#pragma once

class Entity;

class MathUtilities {

public:
	MathUtilities() {}
	~MathUtilities() {}

	static int DistanceBetweenEntities(Entity* e1, Entity* e2);
};