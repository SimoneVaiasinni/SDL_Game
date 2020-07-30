#include "Character.h"
#include "../../TextureLoader.h"
#include "../../World/World.h"
#include "../../Game.h"
#include <fstream>

Character::Character(Location location, Dimension dimension, const char* name)
	: Pawn{ location, dimension, name }
{
	World::Characters.push_back(this);
}

void Character::LoadTextures(const char* FilePath)
{
	std::ifstream stream(FilePath);
	std::string line;

	int index = 0;

	while (std::getline(stream, line))
	{
		//Idle
		//-------
		if (line.find("#idle right") != std::string::npos)
		{
			ImgState = EImageState::idle_R;
			index = 0;
		}
		else if (line.find("#idle left") != std::string::npos)
		{
			ImgState = EImageState::idle_L;
			index = 0;
		}

		//Walk Right
		//-------
		else if (line.find("#walk right") != std::string::npos)
		{
			ImgState = EImageState::walk_R;
			index = 0;
		}
		else if (line.find("#walk left") != std::string::npos)
		{
			ImgState = EImageState::walk_L;
			index = 0;
		}

		//Run 
		//-------
		else if (line.find("#run right") != std::string::npos)
		{
			ImgState = EImageState::run_R;
			index = 0;
		}
		else if (line.find("#run left") != std::string::npos)
		{
			ImgState = EImageState::run_L;
			index = 0;
		}


		//Attack
		//-------
		else if (line.find("#attack right") != std::string::npos)
		{
			ImgState = EImageState::attack_R;
			index = 0;
		}
		else if (line.find("#attack left") != std::string::npos)
		{
			ImgState = EImageState::attack_L;
			index = 0;
		}

		//Hit react
		//-------
		else if (line.find("#hit react right") != std::string::npos)
		{
			ImgState = EImageState::hitReact_R;
			index = 0;
		}
		else if (line.find("#hit react left") != std::string::npos)
		{
			ImgState = EImageState::hitReact_L;
			index = 0;
		}

		//Jump 
		//-------
		else if (line.find("#jump right") != std::string::npos)
		{
			ImgState = EImageState::jump_R;
			index = 0;
		}
		else if (line.find("#jump left") != std::string::npos)
		{
			ImgState = EImageState::jump_L;
			index = 0;
		}

		//Falling
		//-------
		else if (line.find("#falling right") != std::string::npos)
		{
			ImgState = EImageState::falling_R;
			index = 0;
		}
		else if (line.find("#falling left") != std::string::npos)
		{
			ImgState = EImageState::falling_L;
			index = 0;
		}

		//Death
		//-------
		else if (line.find("#death right") != std::string::npos)
		{
			ImgState = EImageState::death_R;
			index = 0;
		}
		else if (line.find("#death left") != std::string::npos)
		{
			ImgState = EImageState::death_L;
			index = 0;
		}
		// --------------------------------------------- //
		else
		{
			if (!line.empty())
			{
				auto Texture = TextureLoader::LoadTexture(line.c_str());

				switch (ImgState)
				{
					//Idle
					//-------
				case EImageState::idle_R:
					Images->Idle_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::idle_L:
					Images->Idle_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

					//Walk
					//-------
				case EImageState::walk_R:
					Images->Walk_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::walk_L:
					Images->Walk_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

					//Run
					//-------
				case EImageState::run_R:
					Images->Run_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::run_L:
					Images->Run_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

					//Attack
					//-------
				case EImageState::attack_R:
					Images->Attack_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::attack_L:
					Images->Attack_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;


					//Attack
					//-------
				case EImageState::hitReact_L:
					Images->HitReact_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::hitReact_R:
					Images->HitReact_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

					//Jump
					//-------
				case EImageState::jump_R:
					Images->Jump_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::jump_L:
					Images->Jump_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;


					//Fallig
					//-------
				case EImageState::falling_R:
					Images->Falling_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::falling_L:
					Images->Falling_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

					//Death
					//-------
				case EImageState::death_R:
					Images->Death_R.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;

				case EImageState::death_L:
					Images->Death_L.insert(std::pair<int, SDL_Texture*>(index, Texture));
					break;


				default:
					break;
				}
				index++;
			}
		}
	}
}

void Character::DrawHealthBar(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int Health)
{
	if (Health <= 0) return;

	SDL_Rect* HealthBar = new SDL_Rect{location.x, location.y - 10, Health, 5};
	SDL_SetRenderDrawColor(Game::Renderer, r, g, b, a);
	SDL_RenderFillRect(Game::Renderer, HealthBar);	
	SDL_SetRenderDrawColor(Game::Renderer, 0, 0, 0, 255);
}