#pragma once
#include "Utilities/BaseMinimal.h"

class TextureLoader
{

public:
	TextureLoader() {}

	static SDL_Texture* LoadTexture(const char* file);
};