#ifndef WIIAUDIO_H
#define WIIAUDIO_H

#include <SDL/SDL_mixer.h>

typedef struct {
	Mix_Music* data;
	
	int loop;
	int used;
} PHL_Music;

typedef struct {
	Mix_Chunk* data;
	
	int loop;
	int used;
} PHL_Sound;

void PHL_AudioInit();
void PHL_AudioClose();

PHL_Music PHL_LoadMusic(char* fname, int loop); //Same as PHL_LoadSound, but expects a file name without extension
PHL_Sound PHL_LoadSound(char* fname);

void PHL_PlayMusic(PHL_Music snd);
void PHL_PlaySound(PHL_Sound snd, int channel);

void PHL_StopMusic();
void PHL_StopSound(PHL_Sound snd, int channel);

void PHL_FreeMusic(PHL_Music snd);
void PHL_FreeSound(PHL_Sound snd);

#endif