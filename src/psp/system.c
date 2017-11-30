#include "system.h"
#include "../text.h"

PSP_MODULE_INFO("Hydra Castle Labrynth", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
//PSP_HEAP_SIZE_KB(12*1024);
PSP_HEAP_SIZE_KB(-1024);

int PHL_MainLoop()
{
	if (quitGame == 1) {
		return 0;
	}	
	return 1;
}

void PHL_ConsoleInit()
{
	pspDebugScreenInit();
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