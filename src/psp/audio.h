#ifndef PSPAUDIO_H
#define PSPAUDIO_H

#include <oslib/oslib.h>

#define PHL_Music OSL_SOUND
#define PHL_Sound OSL_SOUND

void PHL_AudioInit();
void PHL_AudioClose();

OSL_SOUND* PHL_LoadMusic(char* fname, int loop); //Same as PHL_LoadSound, but expects a file name without extension
OSL_SOUND* PHL_LoadSound(char* fname);

void PHL_PlayMusic(OSL_SOUND* snd);
void PHL_PlaySound(OSL_SOUND* snd, int channel);

void PHL_StopMusic();
void PHL_StopSound(OSL_SOUND* snd, int channel);

#define PHL_FreeMusic PHL_FreeSound
void PHL_FreeSound(OSL_SOUND* snd);

#endif