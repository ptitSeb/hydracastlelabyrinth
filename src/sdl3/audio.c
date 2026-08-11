#include "audio.h"
#include "../options.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

int music_volume = 4;

#define MAX_SFX_CHANNELS 8

static MIX_Mixer* mixer = NULL;
static MIX_Track* musicTrack = NULL;
static MIX_Track* sfxTracks[MAX_SFX_CHANNELS] = {NULL};

void PHL_AudioInit()
{
    MIX_Init();
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if(!mixer)
        printf("Error creating SDL_mixer device (%s)\n", SDL_GetError());

    musicTrack = MIX_CreateTrack(mixer);
    for (int i = 0; i < MAX_SFX_CHANNELS; i++)
        sfxTracks[i] = MIX_CreateTrack(mixer);

    PHL_MusicVolume(0.25f * music_volume);
}

void PHL_AudioClose()
{
    MIX_DestroyMixer(mixer);
    mixer = NULL;
    musicTrack = NULL;
    MIX_Quit();
}

//Same as PHL_LoadSound, but expects a file name without extension
PHL_Music PHL_LoadMusic(char* fname, int loop)
{
    PHL_Music ret;
    ret.loop = loop;
    char buff[4096];
    strcpy(buff, "data/");
    strcat(buff, fname);
    strcat(buff, getMusicType()?".ogg":".mid");
    ret.snd = MIX_LoadAudio(mixer, buff, false);
    return ret;
}

PHL_Sound PHL_LoadSound(char* fname)
{
    char buff[4096];
    strcpy(buff, "data/");
    strcat(buff, fname);
    return MIX_LoadAudio(mixer, buff, true);
}

void PHL_MusicVolume(float vol)
{
    if(musicTrack)
        MIX_SetTrackGain(musicTrack, vol);
}

void PHL_PlayMusic(PHL_Music snd)
{
    if(snd.snd && musicTrack) {
        MIX_SetTrackAudio(musicTrack, snd.snd);
        MIX_SetTrackLoops(musicTrack, snd.loop?-1:0);
        MIX_PlayTrack(musicTrack, 0);
    }
}

void PHL_PlaySound(PHL_Sound snd, int channel)
{
    if(snd && channel>=0 && channel<MAX_SFX_CHANNELS) {
        MIX_Track* track = sfxTracks[channel];
        MIX_SetTrackAudio(track, snd);
        MIX_SetTrackLoops(track, 0);
        MIX_PlayTrack(track, 0);
    }
}

void PHL_StopMusic()
{
    if(musicTrack)
        MIX_StopTrack(musicTrack, 0);
}

void PHL_StopSound(PHL_Sound snd, int channel)
{
    if(channel>=0 && channel<MAX_SFX_CHANNELS && sfxTracks[channel])
        MIX_StopTrack(sfxTracks[channel], 0);
}

void PHL_FreeMusic(PHL_Music snd)
{
    if(snd.snd)
        MIX_DestroyAudio(snd.snd);
    snd.snd = NULL;
}

void PHL_FreeSound(PHL_Sound snd)
{
    if(snd)
        MIX_DestroyAudio(snd);
}
