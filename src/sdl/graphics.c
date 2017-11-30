#include <SDL/SDL.h>
#include "../PHL.h"
#include "../game.h"
#include "../qda.h"
#include "graphics.h"

SDL_Surface* screen = NULL;
SDL_Surface* drawbuffer = NULL;
SDL_Surface* backbuffer = NULL;

static uint32_t tframe;

SDL_Color PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Color ret = {.r = r, .b = b, .g = g};
    return ret;
}

void PHL_GraphicsInit()
{
	if ( SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error");
		SDL_Delay(5000);
		exit(EXIT_FAILURE);
	}
	
	SDL_ShowCursor(SDL_DISABLE);
    
    uint32_t flags = SDL_HWSURFACE|SDL_DOUBLEBUF;
    #ifdef PANDORA
    flags |= SDL_FULLSCREEN;
    #endif
    screen = SDL_SetVideoMode(640, 480, 0, flags);
    drawbuffer = screen;
	backbuffer = PHL_NewSurface(640, 480);
	tframe = SDL_GetTicks();
}

void PHL_GraphicsExit()
{
    SDL_FreeSurface(backbuffer);
	SDL_Quit();    
}

void PHL_StartDrawing()
{
	PHL_ResetDrawbuffer();
}
void PHL_EndDrawing()
{
    SDL_Flip(screen);
	uint32_t tnext = tframe + 1000/60;
	while((tframe = SDL_GetTicks())<tnext)
        SDL_Delay(10);
}

void PHL_ForceScreenUpdate()
{

}

void PHL_SetDrawbuffer(PHL_Surface surf)
{
    drawbuffer = surf;
}
void PHL_ResetDrawbuffer()
{
    drawbuffer = screen;
}

//PHL_RGB PHL_NewRGB(int r, int g, int b);
void PHL_SetColorKey(PHL_Surface surf, int r, int g, int b)
{
    SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, r, g, b));
}

PHL_Surface PHL_NewSurface(int w, int h)
{
    return SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}
void PHL_FreeSurface(PHL_Surface surf)
{
    SDL_FreeSurface(surf);
}

//PHL_Surface PHL_LoadBMP(char* fname);
PHL_Surface PHL_LoadBMP(int index)
{
	SDL_Surface* surf;
	
	FILE* f;
	if ( (f = fopen("data/bmp.qda", "rb")) ) {
		uint8_t* QDAFile = (uint8_t*)malloc(headers[index].size);
		fseek(f, headers[index].offset, SEEK_SET);
		fread(QDAFile, 1, headers[index].size, f);
		fclose(f);
		
		PHL_RGB palette[20][18];
		
		uint16_t w, h;
		
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
		
		for (dx = 0; dx < w; dx++) {
			for (dy = 0; dy < h; dy++) {
			
				int pix = dx + w * dy;
				int px = QDAFile[1078 + pix] / 16;
				int py = QDAFile[1078 + pix] % 16;
				//Get transparency from first palette color
				if (dx == 0 && dy == 0) {					
					//Darkness special case
					if (index == 27) {
						SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, 0x00, 0x00, 0x00));
					}else{
						SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, palette[0][0].r, palette[0][0].g, palette[0][0].b));
					}
				}
				
				PHL_RGB c = palette[px][py];
				
				//PHL_DrawRect(dx * 2, dy * 2, 2, 2, c);
				SDL_Rect rect = {dx * 2, (h-1-dy) * 2, 2, 2};	
				SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, c.r, c.g, c.b));
			}
		}
		free(QDAFile);
	}
	
	return surf;
}

void PHL_DrawRect(int x, int y, int w, int h, SDL_Color col)
{
	SDL_Rect rect = {x, y, w, h};
	
	SDL_FillRect(drawbuffer, &rect, SDL_MapRGB(drawbuffer->format, col.r, col.g, col.b));
}

void PHL_DrawSurface(double x, double y, PHL_Surface surface)
{
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= 1;
		} else if (val == 2) {
			y += 1;
		}
	}
	
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(surface, NULL, drawbuffer, &offset);
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
	
	SDL_BlitSurface(surface, &crop, drawbuffer, &offset);
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore)
{
    PHL_DrawSurface(0, 0, backbuffer);
}
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore)
{
	PHL_SetDrawbuffer(backbuffer);
	
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
