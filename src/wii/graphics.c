//Wii graphics.c
#include "graphics.h"
#include "../PHL.h"
#include "../game.h"
#include "../qda.h"
#include <stdio.h>

//One time graphics setup
void PHL_GraphicsInit()
{	
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Error");
		SDL_Delay(5000);
		exit(EXIT_FAILURE);
	}
	
	SDL_ShowCursor(SDL_DISABLE);
	
	surfScreen = SDL_SetVideoMode(640, 480, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
	backBuffer = PHL_NewSurface(640, 480);
	db.fb = surfScreen;
}

void PHL_GraphicsExit()
{
	SDL_FreeSurface(surfScreen);
	SDL_Quit();
	//closeQDA();
}

void PHL_StartDrawing()
{
	/*int i;
	for (i = 0; i < 10; i++) {
		xfb[i] = 0xff;
	}*/
}

void PHL_EndDrawing()
{
	SDL_Flip(surfScreen);
	//frameLimit();
	//VIDEO_Flush();
	//VIDEO_WaitVSync();
	//frameLimit();
}

void PHL_ForceScreenUpdate()
{
	
}

void PHL_SetDrawbuffer(PHL_Surface surf)
{
	db.fb = surf.surf;
}

void PHL_ResetDrawbuffer()
{
	db.fb = surfScreen;
}

PHL_RGB PHL_NewRGB(int r, int g, int b) {
	PHL_RGB col;
	col.r = (unsigned int)r;
	col.g = (unsigned int)g;
	col.b = (unsigned int)b;
	return col;
}

void PHL_SetColorKey(PHL_Surface surf, int r, int g, int b)
{
	Uint32 colorKey = SDL_MapRGB(surf.surf->format, r, g, b);
	SDL_SetColorKey(surf.surf, SDL_SRCCOLORKEY, colorKey);
}

PHL_Surface PHL_NewSurface(int w, int h)
{
	PHL_Surface surf;
	surf.surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	return surf;
}

void PHL_FreeSurface(PHL_Surface surface)
{
	SDL_FreeSurface(surface.surf);
}

PHL_Surface PHL_LoadBMP(int index)
{	
	PHL_Surface surf;
	
	FILE* f;
	if ( (f = fopen("bmp.qda", "rb")) ) {
		unsigned char* QDAFile = (unsigned char*)malloc(headers[index].size);
		fseek(f, headers[index].offset, SEEK_SET);
		fread(QDAFile, 1, headers[index].size, f);
		fclose(f);
		
		FILE* temp;
		if ( (temp = fopen("temp.bmp", "wb")) ) {
			fwrite(QDAFile, sizeof(unsigned char*), sizeof(QDAFile), temp);
		}
		fclose(temp);
		
		PHL_RGB palette[20][18];
		
		unsigned short w, h;
		
		//Read data from header
		memcpy(&w, &QDAFile[18], 2);
		memcpy(&h, &QDAFile[22], 2);
		
		surf = PHL_NewSurface(w * 2, h * 2);
		//surf = PHL_NewSurface(200, 200);

		//Load Palette
		int dx, dy;
		int count = 0;
		for (dx = 0; dx < 20; dx++) {
			for (dy = 0; dy < 16; dy++) {
				palette[dx][dy].b = QDAFile[54 + count];
				palette[dx][dy].g = QDAFile[54 + count + 1];
				palette[dx][dy].r = QDAFile[54 + count + 2];
				count += 4;
			}
		}
		
		for (dx = w; dx > 0; dx--) {
			for (dy = 0; dy < h; dy++) {
			
				int pix = w - dx + w * dy;
				int px = QDAFile[1078 + pix] / 16;
				int py = QDAFile[1078 + pix] % 16;
				
				PHL_RGB c = palette[px][py];
				
				//PHL_DrawRect(dx * 2, dy * 2, 2, 2, c);
				SDL_Rect rect = {dx * 2, dy * 2, 2, 2};	
				SDL_FillRect(surf.surf, &rect, SDL_MapRGB(surf.surf->format, c.r, c.g, c.b));
			}
		}
		free(QDAFile);
	}
	
	fclose(f);
	
	return surf;
}

/*
PHL_Surface PHL_LoadBMP(char* fname)
{
	SDL_Surface* temp;
	SDL_Surface* stretchX;
	SDL_Surface* final;
	
	temp = SDL_LoadBMP(fname);
	int width = temp->w;
	int height = temp->h;
	
	stretchX = SDL_CreateRGBSurface(0, width * 2, height, 32, 0, 0, 0, 0);
	
	SDL_Rect crop, offset;
	offset.y = 0;
	crop.y = 0;
	crop.w = 1;
	crop.h - height;
	
	int i;
	for (i = 0; i < width; i++)
	{
		crop.x = i;
		offset.x = i * 2;
		
		SDL_BlitSurface(temp, &crop, stretchX, &offset);
		offset.x += 1;
		SDL_BlitSurface(temp, &crop, stretchX, &offset);
	}
	
	final = SDL_CreateRGBSurface(0, width * 2, height * 2, 32, 0, 0, 0, 0);
	offset.x = 0; offset.y = 0;
	crop.x = 0; crop.y = 0;
	crop.w = width * 2;
	crop.h = 1;
	
	for (i = 0; i < height; i++)
	{
		crop.y = i;
		offset.y = i * 2;
		
		SDL_BlitSurface(stretchX, &crop, final, &offset);
		offset.y += 1;
		SDL_BlitSurface(stretchX, &crop, final, &offset);
	}
	
	//Cleanup
	SDL_FreeSurface(temp);
	SDL_FreeSurface(stretchX);
	
	PHL_Surface result;
	result.surf = SDL_DisplayFormat(final);
	
	return result;
}
*/

void PHL_DrawRect(int x, int y, int w, int h, PHL_RGB col) {
	SDL_Rect rect = {x, y, w, h};
	
	SDL_FillRect(db.fb, &rect, SDL_MapRGB(db.fb->format, col.r, col.g, col.b));
}

void PHL_DrawSurface(double x, double y, PHL_Surface surface)
{
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= 1;
		}else if (val == 2) {
			y += 1;
		}
	}
	
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(surface.surf, NULL, db.fb, &offset);
}

void PHL_DrawSurfacePart(double x, double y, int cropx, int cropy, int cropw, int croph, PHL_Surface surface)
{
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= 1;
		}else if (val == 2) {
			y += 1;
		}
	}
	
	SDL_Rect crop, offset;
	crop.x = cropx;
	crop.y = cropy;
	crop.w = cropw;
	crop.h = croph;
	
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(surface.surf, &crop, db.fb, &offset);
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore)
{
	PHL_DrawSurface(0, 0, backBuffer);
}

void PHL_UpdateBackground(PHL_Background back, PHL_Background fore)
{
	PHL_SetDrawbuffer(backBuffer);
	
	int xx, yy;
	
	for (yy = 0; yy < 12; yy++)
	{
		for (xx = 0; xx < 16; xx++)
		{
			//Draw Background tiles
			PHL_DrawSurfacePart(xx * 40, yy * 40, back.tileX[xx][yy] * 40, back.tileY[xx][yy] * 40, 40, 40, images[imgTiles]);
			
			//Only draw foreground tile if not a blank tile
			if (fore.tileX[xx][yy] != 0 || fore.tileY[xx][yy] != 0) {
				PHL_DrawSurfacePart(xx * 40, yy * 40, fore.tileX[xx][yy] * 40, fore.tileY[xx][yy] * 40, 40, 40, images[imgTiles]);
			}
		}
	}
	
	PHL_ResetDrawbuffer();
}

/*void frameLimit()
{
	currentTime = SDL_GetTicks() - lastTime;
	if (currentTime < FRAMEWAIT) {
		SDL_Delay(FRAMEWAIT - currentTime);
	}
	lastTime = SDL_GetTicks();
}*/