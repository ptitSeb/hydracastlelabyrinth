#include "audio.h"
#include "../game.h"

void PHL_AudioInit()
{
	oslInitAudio();
}

void PHL_AudioClose()
{
	oslDeinitAudio();
}

//Each system can use a custom music file format
OSL_SOUND* PHL_LoadMusic(char* fname, int loop)
{
	char stream = 0;
	
	OSL_SOUND* result = NULL;

	char fullPath[40];
	strcpy(fullPath, fname);
	strcat(fullPath, ".bgm");
	
	FILE* file;
	
	if ( (file = fopen(fullPath, "r")) ) {
		fclose(file);
		result = oslLoadSoundFile(fullPath, stream);
		oslSetSoundLoop(result, loop);
	}

	return result;
}

OSL_SOUND* PHL_LoadSound(char* fname)
{	
	char stream = 0;
	
	OSL_SOUND* result = NULL;

	char fullPath[40];
	strcpy(fullPath, fname);
	
	FILE* file;
	
	if ( (file = fopen(fullPath, "r")) ) {
		fclose(file);
		result = oslLoadSoundFile(fullPath, stream);
		oslSetSoundLoop(result, 0);
	}

	return result;
}


void PHL_PlayMusic(OSL_SOUND* snd)
{
	//Music always plays on the first channel
	PHL_PlaySound(snd, 0);
}

void PHL_PlaySound(OSL_SOUND* snd, int channel)
{	
	if (snd) {
		oslPlaySound(snd, channel);
	}
}

void PHL_StopMusic()
{
	PHL_StopSound(bgmMusic, 0);
}

void PHL_StopSound(OSL_SOUND* snd, int channel)
{
	if (snd) {
		oslStopSound(snd);
	}
}

void PHL_FreeSound(OSL_SOUND* snd)
{
	if (snd) {
		oslDeleteSound(snd);
	}
}