#pragma once
#include "BaseMinimal.h"
#include <SDL_mixer.h>

#define HIT_EFFECT 0
#define DEATH_EFFECT 1

class SoundComponent {

public:
	SoundComponent(){}
	~SoundComponent(){}

	static void PlayMusic(const char* file);
	static void PlayEffect(Uint8 effect);
};