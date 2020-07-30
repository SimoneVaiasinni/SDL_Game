#include "TextureLoader.h"
#include "Game.h"

SDL_Texture* TextureLoader::LoadTexture(const char* file)
{
	SDL_Surface* imgTemp = IMG_Load(file);
	if (imgTemp)
	{
		auto Texture = SDL_CreateTextureFromSurface(Game::Renderer, imgTemp);
		SDL_FreeSurface(imgTemp);
		return Texture;
	}
	return nullptr;
}