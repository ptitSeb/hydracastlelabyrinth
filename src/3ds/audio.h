#ifndef AUDIO_H
#define AUDIO_H

#include <3ds.h>

typedef struct {
	u16 sampleRate;
	u32 dataSize;	
	u16 bitsPerSample;
	u16 ndspFormat;
	u16 numChannels;
	u8 loop;
	u8* data;
	//int used;
} PHL_Music;

typedef struct{
	u32 sampleRate;
	u32 dataSize;	
	u16 bitsPerSample;
	u16 ndspFormat;
	u16 numChannels;
	u8* data;
} PHL_Sound;

ndspWaveBuf waveBuf[6]; //One for each channel?

void PHL_AudioInit();
void PHL_AudioClose();

PHL_Music PHL_LoadMusic(char* fname, int loop); //Same as PHL_LoadSound, but expects a file name without extension
PHL_Sound PHL_LoadSound(char* fname);

void PHL_PlayMusic(PHL_Music snd);
void PHL_PlaySound(PHL_Sound snd, int channel);

void PHL_StopMusic();
void PHL_StopSound(PHL_Sound snd, int channel);

/*
PHL_Music PHL_LoadMusic(char* fname, int channel); //Same as PHL_LoadSound, but expects a file name without extension
PHL_Sound PHL_LoadSound(char* fname, int loop, int stream, int channel);

void PHL_PlayMusic(PHL_Music snd);
void PHL_PlaySound(PHL_Sound snd);
*/

void PHL_FreeMusic(PHL_Music snd);
void PHL_FreeSound(PHL_Sound snd);

#endif