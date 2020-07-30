#include "SoundComponent.h"

void SoundComponent::PlayMusic(const char* file)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* Music = Mix_LoadMUS(file);
	Mix_PlayMusic(Music, -1);
}

void SoundComponent::PlayEffect(Uint8 effect)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Chunk* sound;

	if (effect == HIT_EFFECT)
		sound = Mix_LoadWAV("./res/sound/hit_effect.wav");
	
	else
		sound = Mix_LoadWAV("./res/sound/death_effect.wav");
	Mix_PlayChannel(-1, sound, false);
}