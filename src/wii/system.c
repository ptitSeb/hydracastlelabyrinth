#include "system.h"
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>

int quitGame = 0;

int PHL_MainLoop()
{
	if (quitGame == 1) {
		return 0;
	}
	return 1;
}

void PHL_ConsoleInit()
{
	//console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
}

void PHL_GameQuit()
{
	quitGame = 1;
}

void PHL_ErrorScreen(char* message)
{
	/*consoleInit(GFX_TOP, NULL);
	
	printf(message);
	printf("\n\nPress START to close");
	
	u32 kDown;
	while (PHL_MainLoop()) {
		//gfxFlushBuffers();
		//gfxSwapBuffers();
		hidScanInput();
		kDown = hidKeysHeld();
		
		if (kDown & KEY_START) { break; } 
		
		gspWaitForVBlank();
	}
	*/
	
	exit(1);
}