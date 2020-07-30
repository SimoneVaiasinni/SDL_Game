#pragma once
#include "../Pawn.h"
#include <map>

class Character : public Pawn {

public:
	Character(Location location, Dimension dimension, const char* name);
	~Character() {};

	virtual void Attack() {}
	virtual void Jump() {}
	virtual void DrawHealthBar(Uint8 r, Uint8 g, Uint8 b, Uint8 a, int Health);

protected:
	enum EImageState {
		idle_R,
		idle_L,

		walk_R,
		walk_L,

		run_R,
		run_L,

		attack_R,
		attack_L,

		hitReact_R,
		hitReact_L,

		jump_R,
		jump_L,

		falling_R,
		falling_L,

		death_R,
		death_L,
	};
	struct CharacterImages {
		std::map<int, SDL_Texture*> Idle_R;
		std::map<int, SDL_Texture*> Idle_L;

		std::map<int, SDL_Texture*> Walk_R;
		std::map<int, SDL_Texture*> Walk_L;

		std::map<int, SDL_Texture*> Run_R;
		std::map<int, SDL_Texture*> Run_L;

		std::map<int, SDL_Texture*> Jump_R;
		std::map<int, SDL_Texture*> Jump_L;

		std::map<int, SDL_Texture*> Falling_R;
		std::map<int, SDL_Texture*> Falling_L;

		std::map<int, SDL_Texture*> Attack_R;
		std::map<int, SDL_Texture*> Attack_L;

		std::map<int, SDL_Texture*> HitReact_R;
		std::map<int, SDL_Texture*> HitReact_L;

		std::map<int, SDL_Texture*> Death_R;
		std::map<int, SDL_Texture*> Death_L;
	};

	EImageState ImgState;
	CharacterImages* Images = new CharacterImages();

	void LoadTextures(const char* FilePath);
};