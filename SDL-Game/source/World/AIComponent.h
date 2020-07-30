#pragma once
#include <list>
class Character;
class Wizard;

class AIComponent {

public:
	AIComponent() {}
	~AIComponent() {}

	void Possess(std::list<Character*> characters);
	void Control();

private:
	std::list<Character*> Characters;
};
