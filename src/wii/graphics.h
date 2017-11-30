//Wii graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

//#include <gccore.h>
#include <SDL/SDL.h>

typedef struct {
	int tileX[16][12];
	int tileY[16][12];
} PHL_Background;

typedef struct {
	unsigned int r;
	unsigned int g;
	unsigned int b;
} PHL_RGB;

typedef struct {
	SDL_Surface* fb;
	int width;
	int height;
} DrawBuffer;

typedef struct {
	SDL_Surface* surf;
} PHL_Surface;

PHL_Surface backBuffer;
SDL_Surface *surfScreen;
DrawBuffer db;

void PHL_GraphicsInit();
void PHL_GraphicsExit();

void PHL_StartDrawing();
void PHL_EndDrawing();

void PHL_ForceScreenUpdate();

void PHL_SetDrawbuffer(PHL_Surface surf);
void PHL_ResetDrawbuffer();

PHL_RGB PHL_NewRGB(int r, int g, int b);
void PHL_SetColorKey(PHL_Surface surf, int r, int g, int b);

PHL_Surface PHL_NewSurface(int w, int h);
void PHL_FreeSurface(PHL_Surface surface);

//PHL_Surface PHL_LoadBMP(char* fname);
PHL_Surface PHL_LoadBMP(int index);

void PHL_DrawRect(int x, int y, int w, int h, PHL_RGB col);

void PHL_DrawSurface(double x, double y, PHL_Surface surface);
void PHL_DrawSurfacePart(double x, double y, int cropx, int cropy, int cropw, int croph, PHL_Surface surface);

void PHL_DrawBackground(PHL_Background back, PHL_Background fore);
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore);

#endif