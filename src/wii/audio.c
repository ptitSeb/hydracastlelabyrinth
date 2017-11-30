#include "audio.h"

void PHL_AudioInit()
{
	//oslInitAudio();	
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096))
	{
		fprintf(stderr, "SDL_Mixer Error: %s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
}

void PHL_AudioClose()
{
	Mix_CloseAudio();
}

//Each system can use a custom music file extension
PHL_Music PHL_LoadMusic(char* fname, int loop)
{	
	PHL_Music result;
	result.loop = loop;

	char fullPath[40];
	strcpy(fullPath, fname);
	strcat(fullPath, ".ogg");
	
	FILE* file;
	if ((file = fopen(fullPath, "r"))) {
		fclose(file);
		
		result.data = Mix_LoadMUS(fullPath);
		result.used = 1;
	}else{
		result.used = 0;
	}
	
	return result;
}

//PHL_Sound PHL_LoadSound(char* fname, int loop, int stream, int channel)
PHL_Sound PHL_LoadSound(char* fname)
{
	PHL_Sound result;
	
	FILE* file;
	if ((file = fopen(fname, "r"))) {
		fclose(file);
		
		result.data = Mix_LoadWAV(fname);
		result.used = 1;
	}else{
		result.used = 0;
	}
	
	return result;
}

void PHL_PlayMusic(PHL_Music snd)
{
	if (snd.used == 1) {
		int loop = 1;
		if (snd.loop == 1) {
			loop = -1;
		}
		Mix_PlayMusic(snd.data, loop);
	}
}

//void PHL_PlaySound(PHL_Sound snd)
void PHL_PlaySound(PHL_Sound snd, int channel)
{
	if (snd.used == 1) {
		Mix_PlayChannel(channel, snd.data, 0);
	}
}

void PHL_StopMusic()
{
	Mix_HaltMusic();
}

void PHL_StopSound(PHL_Sound snd, int channel)
{
	
}

void PHL_FreeMusic(PHL_Music snd)
{
	Mix_FreeMusic(snd.data);
	snd.used = 0;
}

void PHL_FreeSound(PHL_Sound snd)
{
	Mix_FreeChunk(snd.data);
	snd.used = 0;
}