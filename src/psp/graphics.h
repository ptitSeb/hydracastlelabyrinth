#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <pspkernel.h>
#include <oslib/oslib.h>

#define PHL_Surface OSL_IMAGE

#define PHL_RGB OSL_COLOR
#define PHL_NewRGB RGB

typedef struct {
	int tileX[16][12];
	int tileY[16][12];
} PHL_Background;

/*
typedef struct {
	unsigned int r, g, b;
} PHL_RGB;
*/
/*
typedef struct {
	OSL_IMAGE* pxdata;
	int width;
	int height;
	PHL_RGB colorKey;
} PHL_Surface;
*/
OSL_IMAGE* screen;

void PHL_GraphicsInit();
void PHL_GraphicsExit();

void PHL_StartDrawing();
void PHL_EndDrawing();

void PHL_ForceScreenUpdate();

void PHL_SetDrawbuffer(OSL_IMAGE* surf);
void PHL_ResetDrawbuffer();

//PHL_RGB PHL_NewRGB(int r, int g, int b);
void PHL_SetColorKey(OSL_IMAGE* surf, int r, int g, int b);

OSL_IMAGE* PHL_NewSurface(int w, int h);
void PHL_FreeSurface(OSL_IMAGE* surf);

//PHL_Surface PHL_LoadBMP(char* fname);
OSL_IMAGE* PHL_LoadBMP(int index);

void PHL_DrawRect(int x, int y, int w, int h, OSL_COLOR col);

void PHL_DrawSurface(double x, double y, OSL_IMAGE* surface);
void PHL_DrawSurfacePart(double x, double y, int cropx, int cropy, int cropw, int croph, OSL_IMAGE* surface);

void PHL_DrawBackground(PHL_Background back, PHL_Background fore);
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore);

void setBlur(int blur);
void setScreenSize(int size);

int getBlur();
int getScreenSize();

#endif