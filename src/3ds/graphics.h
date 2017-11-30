//3DS graphics.h
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <3ds.h>

typedef struct {
	u8 r;
	u8 g;	
	u8 b;
} PHL_RGB;

typedef struct {
	u8* pxdata;
	u16 width;
	u16 height;
	PHL_RGB colorKey;
} PHL_Surface;

typedef struct {
	gfxScreen_t screen;
	gfx3dSide_t side;
	u16 width;
	u16 height;
} Screen;

typedef struct {
	int tileX[16][12];
	int tileY[16][12];
} PHL_Background;

PHL_Surface db;
Screen* activeScreen;

PHL_Surface backBuffer;

void PHL_GraphicsInit();
void PHL_GraphicsExit();

void PHL_StartDrawing();
void PHL_EndDrawing();

void PHL_ForceScreenUpdate();

void PHL_SetDrawbuffer(PHL_Surface surf);
void PHL_ResetDrawbuffer();

PHL_RGB PHL_NewRGB(u8 r, u8 g, u8 b);
void PHL_SetColorKey(PHL_Surface surf, u8 r, u8 g, u8 b);

PHL_Surface PHL_NewSurface(u16 w, u16 h);
void PHL_FreeSurface(PHL_Surface surf);

PHL_Surface PHL_LoadBMP_Debug(int index);
PHL_Surface PHL_LoadBMP(int index);

void PHL_DrawRect(s16 x, s16 y, u16 w, u16 h, PHL_RGB col);

void PHL_DrawSurface(s16 x, s16 y, PHL_Surface surf);
void PHL_DrawSurfacePart(s16 x, s16 y, s16 cropx, s16 cropy, s16 cropw, s16 croph, PHL_Surface surf);

void PHL_DrawBackground(PHL_Background back, PHL_Background fore);
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore);

void swapScreen(gfxScreen_t screen, gfx3dSide_t side);

#endif