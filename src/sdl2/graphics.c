#include <SDL2/SDL.h>
#include "../PHL.h"
#include "../game.h"
#include "../qda.h"
#include "graphics.h"
#include "scale.h"
#ifdef __amigaos4__
#include "../amigaos.h"
#endif


SDL_Texture* drawbuffer = NULL;
SDL_Texture* screen = NULL;
SDL_Texture* backbuffer = NULL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int wantFullscreen = 0;
int screenScale = 2;
int desktopFS = 0;

int deltaX = 0;
int deltaY = 0;

int screenW = 640;
int screenH = 480;

int actualW = 640;
int actualH = 480;

int drawscreen = 0;

int xbrz = 0;

static const int FPS = 70;

static uint32_t tframe;

extern void Input_InitJoystick();
extern void Input_CloseJoystick();

int getXBRZ()
{
	return xbrz;
}

void setXBRZ(int active)
{
	if(active) active = 1;
	if(xbrz==active) return;
	xbrz = active;

	// try to reload everything, but boss ressource will not be reloaded
	freeImages();
	loadImages();
}


SDL_Color PHL_NewRGB(uint8_t r, uint8_t g, uint8_t b)
{
    SDL_Color ret = {.r = r, .b = b, .g = g};
    return ret;
}

void PHL_GraphicsInit()
{
	SDL_ShowCursor(SDL_DISABLE);

	Input_InitJoystick();
    
    uint32_t flags = 0;
	if(wantFullscreen || desktopFS)
    	flags |= (desktopFS)?SDL_WINDOW_FULLSCREEN_DESKTOP:SDL_WINDOW_FULLSCREEN;
	window = SDL_CreateWindow("HCL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (desktopFS)?0:screenW, (desktopFS)?0:screenH, flags);
	if(!window)
	{
		printf("Error creating SDL Window (%s)\n", SDL_GetError());
		exit(-1);
	}
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
	if(!renderer)
	{
		printf("Error creating SDL Renderer (%s)\n", SDL_GetError());
		SDL_DestroyWindow(window);
		exit(-2);
	}
	if(desktopFS)
	{
		SDL_GetRendererOutputSize(renderer, &screenW, &screenH);
		if(screenW/320 < screenH/240)
			screenScale = screenW/320;
		else
			screenScale = screenH/240; // automatic guess the scale
		deltaX = (screenW-320*screenScale)/2;
		deltaY = (screenH-240*screenScale)/2;
	}
	actualW = 320*screenScale;
	actualH = 240*screenScale;

	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, actualW, actualH);
	drawbuffer = screen;
	drawscreen = 1;

	backbuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, actualW, actualH);
	tframe = SDL_GetTicks();
}

void PHL_GraphicsExit()
{
	Input_CloseJoystick();
	SDL_DestroyTexture(backbuffer);
	SDL_DestroyTexture(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();    
}

void PHL_StartDrawing()
{
	PHL_ResetDrawbuffer();
	/*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);*/
}
void PHL_EndDrawing()
{
	//implement some crude frameskiping, limited to 2 frame skip
	static int skip = 0;
	uint32_t tnext = tframe + 1000/FPS;
	if (SDL_GetTicks()>tnext && skip<2) {
		tframe += 1000/FPS;
		skip++;
		return;
	}

	// handle black borders
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

	SDL_Rect rect = {deltaX, deltaY, actualW, actualH};
	SDL_RenderCopy(renderer, screen, NULL, &rect);

	SDL_RenderPresent(renderer);
	SDL_SetRenderTarget(renderer, drawbuffer);
	while((tframe = SDL_GetTicks())<tnext)
        SDL_Delay(10);
}

void PHL_ForceScreenUpdate()
{

}

void PHL_SetDrawbuffer(SDL_Texture* target)
{
	drawscreen = (target==NULL);
	drawbuffer = target;
	SDL_SetRenderTarget(renderer, drawbuffer);
}
void PHL_ResetDrawbuffer()
{
	drawbuffer = screen;
	drawscreen = 1;
	SDL_SetRenderTarget(renderer, drawbuffer);
}

//PHL_RGB PHL_NewRGB(int r, int g, int b);
void PHL_SetColorKey(PHL_Surface surf, int r, int g, int b)
{
    SDL_SetColorKey(surf.surf, SDL_TRUE, SDL_MapRGB(surf.surf->format, r, g, b));
	if(surf.tex)
		SDL_DestroyTexture(surf.tex);
	surf.tex = SDL_CreateTextureFromSurface(renderer, surf.surf);
}

PHL_Surface PHL_NewSurface(int w, int h)
{
	PHL_Surface ret = {0};
	if(getXBRZ())
		ret.surf = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	else
    	ret.surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	ret.tex = NULL;
	return ret;
}
void PHL_FreeSurface(PHL_Surface surf)
{
	if(surf.tex)
		SDL_DestroyTexture(surf.tex);
	if(surf.surf)
    	SDL_FreeSurface(surf.surf);
}

//PHL_Surface PHL_LoadBMP(char* fname);
PHL_Surface PHL_LoadBMP(int index)
{
	PHL_Surface surf;
	
	FILE* f;
	if ( (f = fopen("data/bmp.qda", "rb")) ) {
		uint8_t* QDAFile = (uint8_t*)malloc(headers[index].size);
		fseek(f, headers[index].offset, SEEK_SET);
		int tmp = fread(QDAFile, 1, headers[index].size, f);
		fclose(f);
		
		uint16_t w, h;
		
		//Read data from header
		memcpy(&w, &QDAFile[18], 2);
		memcpy(&h, &QDAFile[22], 2);
		#ifdef __amigaos4__
		BE16(&w); BE16(&h);
		#endif
		
        surf = PHL_NewSurface(w * screenScale, h * screenScale);
		//surf = PHL_NewSurface(200, 200);

		//Load Palette
		int dx, dy;
		int count = 0;

		if(getXBRZ()) {
			Uint32 palette[20][18];

			for (dx = 0; dx < 20; dx++) {
				for (dy = 0; dy < 16; dy++) {
					palette[dx][dy] = 255<<24 | ((Uint32)QDAFile[54 + count])<<0 | ((Uint32)QDAFile[54 + count + 1])<<8 | ((Uint32)QDAFile[54 + count + 2])<<16;
					count += 4;
				}
			}
			Uint32* tmp = NULL;
			tmp = (Uint32*)malloc(w*h*screenScale*4);
			Uint32 transp;
			for (dx = 0; dx < w; dx++) {
				for (dy = 0; dy < h; dy++) {
				
					int pix = dx + w * dy;
					int px = QDAFile[1078 + pix] / 16;
					int py = QDAFile[1078 + pix] % 16;
					//Get transparency from first palette color
					if (dx == 0 && dy == 0) {					
						//Darkness special case
						if (index == 27)
							transp = 255<<24;
						else
							transp = palette[0][0];
					}
					
					Uint32 c = palette[px][py];
					if(c==transp)
						c=0;
					tmp[(h-1-dy)*w+dx] = c;
				}
			}

			xbrz_scale((void*)tmp, (void*)surf.surf->pixels, w, h, screenScale);
			free(tmp);
		} else {
			PHL_RGB palette[20][18];

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
							SDL_SetColorKey(surf.surf, SDL_TRUE, SDL_MapRGB(surf.surf->format, 0x00, 0x00, 0x00));
						}else{
							SDL_SetColorKey(surf.surf, SDL_TRUE, SDL_MapRGB(surf.surf->format, palette[0][0].r, palette[0][0].g, palette[0][0].b));
						}
					}
					
					PHL_RGB c = palette[px][py];
					//PHL_DrawRect(dx * 2, dy * 2, 2, 2, c);
					SDL_Rect rect = {dx * screenScale, (h-1-dy) * screenScale, screenScale, screenScale};	
					SDL_FillRect(surf.surf, &rect, SDL_MapRGB(surf.surf->format, c.r, c.g, c.b));
				}
			}
		}
		free(QDAFile);
	}
	
	surf.tex = SDL_CreateTextureFromSurface(renderer, surf.surf);
	return surf;
}

void PHL_DrawRect(int x, int y, int w, int h, SDL_Color col)
{
	SDL_Rect rect = {x*screenScale/2, y*screenScale/2, w*screenScale/2, h*screenScale/2};
	
	SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_RenderFillRect(renderer, &rect);
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
	offset.x = x*screenScale/2;
	offset.y = y*screenScale/2;
	offset.w = surface.surf->w;
	offset.h = surface.surf->h;
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, surface.tex, NULL, &offset);
}
void PHL_DrawSurfacePart(double x, double y, int cropx, int cropy, int cropw, int croph, PHL_Surface surface)
{
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= (screenScale==1)?2:1;
		}else if (val == 2) {
			y += (screenScale==1)?2:1;
		}
	}
	
	SDL_Rect crop, offset;
	crop.x = cropx*screenScale/2;
	crop.y = cropy*screenScale/2;
	crop.w = cropw*screenScale/2;
	crop.h = croph*screenScale/2;
	
	offset.x = x*screenScale/2;
	offset.y = y*screenScale/2;
	offset.w = crop.w;
	offset.h = crop.h;
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, surface.tex, &crop, &offset);
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_RenderCopy(renderer, backbuffer, NULL, NULL);
}
void PHL_UpdateBackground(PHL_Background back, PHL_Background fore)
{
	PHL_SetDrawbuffer(backbuffer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
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
