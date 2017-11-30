//3DS graphics.c
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../game.h"
#include "../qda.h"
#include "../hero.h"

Screen scrnTop = { GFX_TOP,
				   GFX_LEFT,
				   400,
				   240 };

Screen scrnBottom = { GFX_BOTTOM,
					  GFX_LEFT,
					  320,
					  240 };

PHL_Rect offset/*, crop*/;
					  
//One time graphics setup
void PHL_GraphicsInit()
{
	gfxInitDefault();
	
	activeScreen = &scrnTop;
	/*
	offset.w = 320;
	offset.h = 240;
	offset.x = (activeScreen->width - offset.w) / 2;
	offset.y = (activeScreen->height - offset.h) / 2;
	
	crop.w = 320;
	crop.h = 320;
	crop.x = 0;
	crop.y = 0;
	
	db.width = activeScreen->width;
	db.height = activeScreen->height;
	*/
	PHL_ResetDrawbuffer();
	
	//Create background image
	backBuffer = PHL_NewSurface(640, 480);
	
	//consoleInit(GFX_BOTTOM, NULL);
	//printf("Console Init");
}

void PHL_GraphicsExit()
{
	gfxExit();
}

void PHL_StartDrawing()
{
	PHL_ResetDrawbuffer();
	
	gfxFlushBuffers();
}

void PHL_EndDrawing()
{	
	gfxSwapBuffers();
	gspWaitForVBlank();
}

void PHL_ForceScreenUpdate()
{	
	//Draw black borders on sides of top screen
	/*
	PHL_DrawRect(-80, 0, 80, 480, PHL_NewRGB(0, 0, 0));
	PHL_DrawRect(640, 0, 80, 480, PHL_NewRGB(0, 0, 0));
	
	gspWaitForVBlank();	
	gfxSwapBuffers();
	
	gfxFlushBuffers();
	swapScreen(GFX_TOP, GFX_LEFT);
	*/
	
	//PHL_DrawRect(0, 0, 640, 48, PHL_NewRGB(0, 0, 0));
	//gfxSwapBuffers();
}

void PHL_SetDrawbuffer(PHL_Surface surf)
{
	db = surf;
	
	offset.w = db.width;
	offset.h = db.height;
	offset.x = 0;
	offset.y = 0;
	/*
	crop.w = db.width;
	crop.h = db.height;
	crop.x = 0;
	crop.y = 0;
	*/
}

void PHL_ResetDrawbuffer()
{
	db.width  = activeScreen->width;
	db.height = activeScreen->height;
	db.pxdata = gfxGetFramebuffer(activeScreen->screen, activeScreen->side, NULL, NULL);
	
	offset.w = 320;
	offset.h = 240;
	offset.x = (activeScreen->width - offset.w) / 2;
	offset.y = (activeScreen->height - offset.h) / 2;
	/*
	crop.w = 320;
	crop.h = 240;
	crop.x = 0;
	crop.y = 0;*/
}

PHL_RGB PHL_NewRGB(u8 r, u8 g, u8 b)
{
	PHL_RGB c = { r, g, b };
	return c;
}

void PHL_SetColorKey(PHL_Surface surf, u8 r, u8 g, u8 b)
{
	PHL_RGB key = { r, g, b };
	surf.colorKey = key;
}

PHL_Surface PHL_NewSurface(u16 w, u16 h)
{
	PHL_Surface surf;
	
	surf.width = w / 2;
	surf.height = h / 2;
	//printf("\nWidth: %d", surf.width);
	surf.pxdata = malloc(surf.width * surf.height * 3 * sizeof(u8));
	surf.colorKey = PHL_NewRGB(0xFF, 0x00, 0xFF);
	
	return surf;
}

void PHL_FreeSurface(PHL_Surface surf)
{
	if (surf.pxdata != NULL) {
		free(surf.pxdata);
		surf.pxdata = NULL;
	}
}

PHL_Surface PHL_LoadBMP(int index)
{	
	PHL_Surface surf;

	FILE* f;
	if ( (f = fopen("romfs:/bmp.qda", "rb")) )
	{
		//Save bmp data
		u8* bmpFile = malloc(headers[index].size * sizeof(u8));
		
		fseek(f, headers[index].offset, SEEK_SET);			
		fread(bmpFile, headers[index].size, 1, f);
		fclose(f);
		
		//Create surface
		u16 w, h;
		memcpy(&w, &bmpFile[18], 2);
		memcpy(&h, &bmpFile[22], 2);
		
		surf = PHL_NewSurface(w * 2, h * 2);
		
		//Load Palette
		PHL_RGB palette[20][18];
			
		int count = 0;
		for (int dx = 0; dx < 20; dx++)
		{
			for (int dy = 0; dy < 16; dy++)
			{
				palette[dx][dy].b = bmpFile[54 + count];
				palette[dx][dy].g = bmpFile[54 + count + 1];
				palette[dx][dy].r = bmpFile[54 + count + 2];

				count += 4;
			}
		}
		
		//Fill pixels
		count = 0;
		for (int dx = w; dx > 0; dx--)
		{
			for (int dy = 0; dy < h; dy++)
			{
				int pix = w - dx + w * dy;
				int px = bmpFile[1078 + pix] / 16;
				int py = bmpFile[1078 + pix] % 16;
				
				//Get transparency from first palette color
				if (dx == w && dy == 0) {					
					//Darkness special case
					if (index == 27) {
						surf.colorKey = PHL_NewRGB(0x00, 0x00, 0x00);
					}else{
						surf.colorKey = palette[0][0];
					}
				}
				
				PHL_RGB c = palette[px][py];
				surf.pxdata[count]   = c.b;
				surf.pxdata[count+1] = c.g;
				surf.pxdata[count+2] = c.r;
				
				count += 3;
			}
		}
		
		//Cleanup
		free(bmpFile);
	}

	return surf;
}

void PHL_DrawRect(s16 x, s16 y, u16 w, u16 h, PHL_RGB col)
{	
	//Quake Shake
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= 2;
		}else if (val == 2) {
			y += 2;
		}
	}

	//Shrink values for small 3ds screen
	x /= 2;	
	y /= 2;
	
	x += offset.x;
	y += offset.y;
	
	w /= 2;
	h /= 2;
	
	s16 x2 = x + w;
	s16 y2 = y + h;
	
	//Keep drawing within screen
	if (x < offset.x) { x = offset.x; }
	if (y < offset.y) { y = offset.y; }
	if (x2 > offset.x + offset.w) { x2 = offset.x + offset.w; }
	if (y2 > offset.y + offset.h) { y2 = offset.y + offset.h; }
	
	w = x2 - x;
	h = y2 - y;
	
	u32 p = ((db.height - h - y) + (x * db.height)) * 3;
	
	for (int i = 0; i < w; i++)
	{
		for (int a = 0; a < h; a++)
		{			
			db.pxdata[p] = col.b;
			db.pxdata[p+1] = col.g;
			db.pxdata[p+2] = col.r;
			
			p += 3;
		}
		p += (db.height - h) * 3;
	}
}

void PHL_DrawSurface(s16 x, s16 y, PHL_Surface surf)
{
	PHL_DrawSurfacePart(x, y, 0, 0, surf.width * 2, surf.height * 2, surf);
}

void PHL_DrawSurfacePart(s16 x, s16 y, s16 cropx, s16 cropy, s16 cropw, s16 croph, PHL_Surface surf)
{	
	if (surf.pxdata != NULL)
	{
		//Quake Shake
		if (quakeTimer > 0) {
			int val = quakeTimer % 4;
			if (val == 0) {
				y -= 2;
			}else if (val == 2) {
				y += 2;
			}
		}
		
		//Shrink values for small 3ds screen
		x = (int)x / 2;	
		y = (int)y / 2;
		
		cropx = cropx / 2;
		cropy = cropy / 2;
		cropw /= 2;
		croph /= 2;
		
		if (x > offset.w || y > offset.h || x + cropw < 0 || y + croph < 0) {
			//image is outside of screen, so don't bother drawing
		}
		else{				
			//Crop pixels that are outside of screen
			if (x < 0) {
				cropx += -(x);
				cropw -= -(x);
				x = 0;
			}
			if (y < 0) {
				cropy += -(y);
				croph -= -(y);
				y = 0;
			}
			
			//3DS exclusive optimization
			if (roomDarkness == 1) {
			//if (1) {
				int cornerX = (herox / 2) - 80;
				int cornerY = (heroy / 2) + 10 - 80;
				
				if (x < cornerX) {
					cropx += cornerX - x;
					cropw -= cornerX - x;
					x = cornerX;
				}
				if (y < cornerY) {
					cropy += cornerY - y;
					croph -= cornerY - y;
					y = cornerY;
				}
				if (x + cropw > cornerX + 160) {
					cropw -= (x + cropw) - (cornerX + 160);
				}
				if (y + croph > cornerY + 160) {
					croph -= (y + croph) - (cornerY + 160);
				}
			}	

			if (x + cropw > offset.w) {
				cropw -= (x + cropw) - (offset.w);
			}
			if (y + croph > offset.h) {
				croph -= (y + croph) - (offset.h);
			}
		
			x += offset.x;
			y += offset.y;			
			
			u32 p = ((offset.h - croph - y) + (x * offset.h)) * 3;
			u32 c = ((surf.height - cropy - croph) + surf.height * cropx) * 3;
			
			for (int i = 0; i < cropw; i++)
			{
				for (int a = 0; a < croph; a++)
				{
					if (surf.colorKey.r != surf.pxdata[c+2] ||
						surf.colorKey.g != surf.pxdata[c+1] ||
						surf.colorKey.b != surf.pxdata[c] )
					{
					
						db.pxdata[p] = surf.pxdata[c];
						db.pxdata[p+1] = surf.pxdata[c+1];
						db.pxdata[p+2] = surf.pxdata[c+2];
					}
					
					c += 3;
					p += 3;
				}
				
				p += (offset.h - croph) * 3;
				c += (surf.height - croph) * 3;				
			}
		}
	}
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore)
{
	PHL_DrawSurface(0, 0, backBuffer);
}

void PHL_UpdateBackground(PHL_Background back, PHL_Background fore)
{
	int tempDarkness = roomDarkness;
	roomDarkness = 0;
	
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
	
	roomDarkness = tempDarkness;
}

//3DS exclusive. Changes which screen to draw on
void swapScreen(gfxScreen_t screen, gfx3dSide_t side)
{
	//Clear old screen
	PHL_StartDrawing();
	PHL_DrawRect(0, 0, 640, 480, PHL_NewRGB(0, 0, 0));
	PHL_EndDrawing();
	PHL_StartDrawing();
	PHL_DrawRect(0, 0, 640, 480, PHL_NewRGB(0, 0, 0));
	PHL_EndDrawing();
	
	if (screen == GFX_TOP)
	{
		activeScreen = &scrnTop;
	}
	else{
		activeScreen = &scrnBottom;
	}
	
	PHL_ResetDrawbuffer();
}