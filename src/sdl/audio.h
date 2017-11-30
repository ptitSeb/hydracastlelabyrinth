#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include <SDL/SDL_mixer.h>

//#define PHL_Music Mix_Music*
#define PHL_Sound Mix_Chunk*
typedef struct
{
    int         loop;
    Mix_Music*  snd;

} PHL_Music;

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