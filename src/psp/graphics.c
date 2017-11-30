#include "graphics.h"
#include "../game.h"
#include "../qda.h"

int screenOffsetX = 80;
int screenOffsetY = 16;
int fullscreen = 0;
int blurFilter = 0;

//One time graphics setup
void PHL_GraphicsInit()
{
	oslInit(0);
	oslInitGfx(OSL_PF_8888, 1);

	screen = oslCreateImage(320, 240, OSL_IN_VRAM, OSL_PF_5650);
	
	PHL_StartDrawing();
}

void PHL_GraphicsExit()
{
	oslEndGfx();
	oslDeleteImage(screen);
	
	//closeQDA();
}

void PHL_StartDrawing()
{
	oslStartDrawing();
	oslSetDrawBuffer(screen);
}

void PHL_EndDrawing()
{
	oslSetDrawBuffer(OSL_DEFAULT_BUFFER);

	oslSetBilinearFilter(blurFilter);
	oslDrawImageXY(screen, screenOffsetX, screenOffsetY);
	oslSetBilinearFilter(0);
	
	oslEndDrawing();
	oslEndFrame();
	oslSyncFrame();
}

void PHL_ForceScreenUpdate()
{
	oslSetDrawBuffer(OSL_DEFAULT_BUFFER);
	oslDrawImageXY(screen, screenOffsetX, screenOffsetY);	
	oslEndDrawing();
	oslAudioVSync();
	oslSyncFrame();
	
	oslStartDrawing();
	oslSetDrawBuffer(screen);
}

void PHL_SetDrawbuffer(OSL_IMAGE* surf)
{
	//unused in psp version
}

void PHL_ResetDrawbuffer()
{
	//unused in psp version
}

void PHL_SetColorKey(OSL_IMAGE* surf, int r, int g, int b)
{
	
}

OSL_IMAGE* PHL_NewSurface(int w, int h)
{
	OSL_IMAGE* surf = oslCreateImage(w, h, OSL_IN_RAM, OSL_PF_8BIT);
	
	return surf;
}

void PHL_FreeSurface(OSL_IMAGE* surf)
{
	if (surf) {
		oslDeleteImage(surf);
		surf = NULL;
	}
}

OSL_IMAGE* PHL_LoadBMP(int index)
{
	OSL_IMAGE* result = NULL;
	
	FILE* f;
	if ( (f = fopen("bmp.qda", "rb")) ) {
		//Load QDA file data
		unsigned char* QDAFile = (unsigned char*)malloc(headers[index].size);
		fseek(f, headers[index].offset, SEEK_SET);
		fread(QDAFile, headers[index].size, 1, f);
		
		OSL_COLOR palette[20][18];
		
		//Read data from header
		unsigned short w, h;
				
		memcpy(&w, &QDAFile[18], 2);
		memcpy(&h, &QDAFile[22], 2);
		
		result = oslCreateImage(w, h, OSL_IN_RAM, OSL_PF_8888);
		oslUnswizzleImage(result);
		oslClearImage(result, RGBA(255, 255, 255, 255));
		
		//Load Palette
		int dx, dy;
		int count = 0;
		for (dx = 0; dx < 20; dx++) {
			for (dy = 0; dy < 16; dy++) {
				palette[dx][dy] = RGB(QDAFile[54 + count + 2], QDAFile[54 + count + 1], QDAFile[54 + count]);
				count += 4;
			}
		}
		
		OSL_COLOR alphaKey = palette[0][0];
		
		//Darkness special case
		if (index == 27) {
			alphaKey = RGB(0, 0, 0);
		}
		
		//Edit surface pixels
		oslLockImage(result);
		for (dx = w; dx > 0; dx--) {
			for (dy = h; dy >= 0; dy--) {		
				int pix = w - dx + w * dy;

				int px = QDAFile[1078 + pix] / 16;
				int py = QDAFile[1078 + pix] % 16;
				
				if (palette[px][py] == alphaKey) {
					oslSetImagePixel(result, w - dx, h - dy - 1, RGBA(0, 0, 0, 0));
				}else{					
					oslSetImagePixel(result, w - dx, h - dy - 1, palette[px][py]);
				}			
			}
		}
		oslUnlockImage(result);
		
		oslSwizzleImage(result);
		
		free(QDAFile);
	}
	
	fclose(f);
	
	return result;
}

void PHL_DrawRect(int x, int y, int w, int h, OSL_COLOR col)
{
	oslDrawFillRect(x/2, y/2, (x + w) / 2, (y + h) / 2, col);
}

void PHL_DrawSurface(double x, double y, OSL_IMAGE* surface)
{
	oslDrawImageXY(surface, (int)(x/2), (int)(y/2));
}

void PHL_DrawSurfacePart(double x, double y, int cropx, int cropy, int cropw, int croph, OSL_IMAGE* surface)
{
	//Consider earthquake
	if (quakeTimer > 0) {
		int val = quakeTimer % 4;
		if (val == 0) {
			y -= 2;
		}else if (val == 2) {
			y += 2;
		}
	}
	
	OSL_IMAGE *crop = oslCreateImageTileSize(surface, cropx/2, cropy/2, cropw/2, croph/2);
	oslDrawImageXY(crop, (int)(x/2), (int)(y/2));
	oslDeleteImage(crop);
}

void PHL_DrawBackground(PHL_Background back, PHL_Background fore)
{
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
}

void PHL_UpdateBackground(PHL_Background back, PHL_Background fore)
{
	//Unused in psp version
}

void setBlur(int blur)
{
	blurFilter = blur;
}

int getBlur()
{
	return blurFilter;
}

void setScreenSize(int size)
{
	int screenW = 320;
	int screenH = 240;
	
	if (size == 1) {
		screenW = 368;
		screenH = 272;		
	}
	if (size == 2) {
		screenW = 480;
		screenH = 272;
	}
	
	screenOffsetX = (480 - screenW) / 2;
	screenOffsetY = (272 - screenH) / 2;
	
	screen->stretchX = screenW;
	screen->stretchY = screenH;
}

int getScreenSize()
{
	if (screen->stretchX == 368) {
		return 1;
	}
	if (screen->stretchX == 480) {
		return 2;
	}
	return 0;
}