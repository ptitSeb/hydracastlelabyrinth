#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MODE_NDSP = 1,
		  MODE_CSND = 2;

int audioMode = 0;

PHL_Sound loadWav(char* fname);
PHL_Music loadRaw(char* fname, u32 sampleRate, u32 bitsPerSample, u16 numChannels, u8 loop);

void PHL_AudioInit()
{
	int loadMode = 0;
	
	Result rdsp = ndspInit();
	if (rdsp) {
		//printf("\nNDSP Failed: %08lX\n", rdsp);
		//loadMode = MODE_CSND;
		loadMode = 0; //DSP or no sound for you! CSND a shit.
	}else{
		loadMode = MODE_NDSP;
	}
	
	if (loadMode == MODE_NDSP) {
		//printf("\nNDSP Audio Mode");
		
		ndspSetOutputMode(NDSP_OUTPUT_STEREO);
		ndspSetOutputCount(1);
		
		audioMode = MODE_NDSP;
	}
	
	else if (loadMode == MODE_CSND) {
		//printf("\nCSND Audio Mode");
		
		csndInit();
		audioMode = MODE_CSND;
	}
}

void PHL_AudioClose()
{
	if (audioMode == MODE_NDSP) {
		ndspExit();
	}
	
	else if (audioMode == MODE_CSND) {
		csndExit();
	}
}

//Each system can use a custom music file format
PHL_Music PHL_LoadMusic(char* fname, int loop)
{	
	return loadRaw(fname, 32000, 16, 1, loop);
}

PHL_Music loadRaw(char* fname, u32 sampleRate, u32 bitsPerSample, u16 numChannels, u8 loop)
{
	PHL_Music result;
	
	char fullPath[128];
	strcpy(fullPath, "romfs:/");
	strcat(fullPath, fname);
	strcat(fullPath, ".raw");
	
	FILE* file;
	if ((file = fopen(fullPath, "rb")))
	{		
		u32 size;
		
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		
		fseek(file, 0, SEEK_SET);
	
		result.sampleRate = sampleRate;
		result.dataSize = size;
		result.bitsPerSample = bitsPerSample;
		result.numChannels = numChannels;
		
		if(bitsPerSample == 8) {
			result.ndspFormat = (numChannels == 1) ?
				NDSP_FORMAT_MONO_PCM8 :
				NDSP_FORMAT_STEREO_PCM8;
		}
		else {
			result.ndspFormat = (numChannels == 1) ?
				NDSP_FORMAT_MONO_PCM16 :
				NDSP_FORMAT_STEREO_PCM16;
		}
		
		result.loop = loop;
		
		result.data = linearAlloc(size);
		fread(result.data, 1, size, file);
		
		fclose(file);
	}
	
	return result;
	
}

PHL_Sound PHL_LoadSound(char* fname)
{
	return loadWav(fname);
}

PHL_Sound loadWav(char* fname)
{
	PHL_Sound snd;
	
	char fullPath[128];
	strcpy(fullPath, "romfs:/");
	strcat(fullPath, fname);
	
	FILE* f;
	if ( (f = fopen(fullPath, "rb")) )
	{
		//Check for valid fileName
		u32 sig;
		fread(&sig, 4, 1, f);
		
		if (sig == 0x46464952) //RIFF
		{
			u32 chunkSize;
			u32 format;
			
			fread(&chunkSize, 4, 1, f);
			fread(&format, 4, 1, f);
			
			if (format == 0x45564157) //WAVE
			{
				u32 subchunk1ID;
				fread(&subchunk1ID, 4, 1, f);
				
				if (subchunk1ID == 0x20746D66) //fmt
				{
					u32 subchunk1Size;
					u16 audioFormat;
					u16 numChannels;
					u32 sampleRate;
					u32 byteRate;
					u16 blockAlign;
					u16 bitsPerSample;
					
					fread(&subchunk1Size, 4, 1, f);
					fread(&audioFormat, 2, 1, f);
					fread(&numChannels, 2, 1, f);
					fread(&sampleRate, 4, 1, f);
					fread(&byteRate, 4, 1, f);
					fread(&blockAlign, 2, 1, f);
					fread(&bitsPerSample, 2, 1, f);
					
					//Search for 'data'
					for (int i = 0; i < 100; i++) {
						u8 c;
						fread(&c, 1, 1, f);
						if (c == 0x64) { //'d'
							fread(&c, 1, 1, f);
							if (c == 0x61) { //'a'
								fread(&c, 1, 1, f);
								if (c == 0x74) { //'t'
									fread(&c, 1, 1, f);
									if (c == 0x61) { //'a'
										i = 100;
									}
								}
							}
						}						
					}
					
					u32 subchunk2Size;
					fread(&subchunk2Size, 4, 1, f);
					
					snd.numChannels = numChannels;
					
					if(bitsPerSample == 8)
					{
						snd.ndspFormat = (numChannels == 1) ?
							NDSP_FORMAT_MONO_PCM8 :
							NDSP_FORMAT_STEREO_PCM8;
					}
					else
					{
						snd.ndspFormat = (numChannels == 1) ?
							NDSP_FORMAT_MONO_PCM16 :
							NDSP_FORMAT_STEREO_PCM16;
					}
					
					snd.sampleRate = sampleRate;
					snd.dataSize = subchunk2Size;
					snd.bitsPerSample = bitsPerSample;
					
					snd.data = (u8*)(linearAlloc(subchunk2Size));					
					fread(snd.data, 1, subchunk2Size, f);
				}
			}
		}
		
		fclose(f);
	}
	
	return snd;
}

void PHL_PlayMusic(PHL_Music snd)
{
	if (snd.data != NULL) {
		if (audioMode == MODE_NDSP)
		{
			ndspChnReset(0);
			ndspChnSetInterp(0, NDSP_INTERP_NONE);
			ndspChnSetRate(0, (float)snd.sampleRate);
			ndspChnSetFormat(0, snd.ndspFormat);
			
			
			memset(&waveBuf[0], 0, sizeof(ndspWaveBuf));
			waveBuf[0].data_vaddr = (u32)(snd.data);
			waveBuf[0].nsamples = snd.dataSize / (snd.bitsPerSample >> 3) / snd.numChannels;
			waveBuf[0].looping = snd.loop;
			waveBuf[0].status = NDSP_WBUF_FREE;
			
			DSP_FlushDataCache(snd.data, snd.dataSize);
			ndspChnWaveBufAdd(0, &waveBuf[0]);
		}
		
		else if (audioMode == MODE_CSND)
		{
			u32 flags = SOUND_FORMAT_16BIT;
		
			if (snd.loop == 1) {
				flags = SOUND_FORMAT_16BIT | SOUND_REPEAT;
			}
			
			int channel = (0 + 1) * 8;
			csndPlaySound(channel, flags, snd.sampleRate, 1, 0, snd.data, snd.data, snd.dataSize);
		}
	}
}

void PHL_PlaySound(PHL_Sound snd, int channel)
{
	if (snd.data != NULL) {
		if (audioMode == MODE_NDSP)
		{
			ndspChnReset(channel);
			ndspChnSetInterp(channel, NDSP_INTERP_NONE);
			ndspChnSetRate(channel, (float)snd.sampleRate);
			ndspChnSetFormat(channel, snd.ndspFormat);
			
			
			memset(&waveBuf[channel], 0, sizeof(ndspWaveBuf));
			waveBuf[channel].data_vaddr = (u32)(snd.data);
			waveBuf[channel].nsamples = snd.dataSize / (snd.bitsPerSample >> 3) / snd.numChannels;
			waveBuf[channel].looping = false;
			waveBuf[channel].status = NDSP_WBUF_FREE;
			
			DSP_FlushDataCache(snd.data, snd.dataSize);
			ndspChnWaveBufAdd(channel, &waveBuf[channel]);
		}
		
		else if (audioMode == MODE_CSND)
		{
			channel = (channel + 1) * 8;
			csndPlaySound(channel, SOUND_FORMAT_16BIT, snd.sampleRate, 1, 0, snd.data, snd.data, snd.dataSize);
		}
	}
}

void PHL_StopMusic()
{
	if (audioMode == MODE_NDSP)
	{
		ndspChnWaveBufClear(0);
	}
	
	else if (audioMode == MODE_CSND)
	{
		CSND_SetPlayState(8, 0);
	}
}

void PHL_StopSound(PHL_Sound snd, int channel)
{
	if (audioMode == MODE_NDSP)
	{
		ndspChnWaveBufClear(channel);
	}
	
	else if (audioMode == MODE_CSND)
	{
		channel = (channel + 1) * 8;
		CSND_SetPlayState(channel, 0);
	}
}

void PHL_FreeMusic(PHL_Music snd)
{
	if (snd.data != NULL) {
		linearFree(snd.data);
		snd.data = NULL;
	}
}

void PHL_FreeSound(PHL_Sound snd)
{
	if (snd.data != NULL) {
		linearFree(snd.data);
		snd.data = NULL;
	}
}