#include "PHL.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#ifdef ODROID
#define _XTYPEDEF_MASK
#include <X11/Xlib.h>
#endif

void createSaveLocations()
{	
	//Force create save data folders	
	#ifdef _3DS
		//3DS builds
		mkdir("sdmc:/3ds", 0777);
		mkdir("sdmc:/3ds/appdata", 0777);
		mkdir("sdmc:/3ds/appdata/HydraCastleLabyrinth", 0777);
		mkdir("sdmc:/3ds/appdata/HydraCastleLabyrinth/data", 0777);
		mkdir("sdmc:/3ds/appdata/HydraCastleLabyrinth/map", 0777);
	#elif defined(_SDL)
		char buff[4096];
		#ifdef __amigaos4__
		strcpy(buff,"PROGDIR:.hydracastlelabyrinth");
		#else
		strcpy(buff, getenv("HOME"));
		strcat(buff, "/.hydracastlelabyrinth");
		#endif
		// if exist first?
		struct stat sb;
		if(!(stat(buff, &sb)==0 && S_ISDIR(sb.st_mode)))
			mkdir(buff, 0777);

	#else
		//psp, wii
		mkdir("/data", 0777);
		mkdir("/map", 0777);
	#endif
}


int main(int argc, char **argv)
{	
	//Setup
	#ifdef _3DS
		sdmcInit();
		osSetSpeedupEnable(false);
	#endif
	#ifdef _SDL
	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0) {
		fprintf(stderr, "Error");
		SDL_Delay(5000);
		exit(EXIT_FAILURE);
	}
	#if defined(PANDORA) || defined(PYRA) || defined(CHIP) || defined(ODROID)
	wantFullscreen = 1;
	#else
	wantFullscreen = 0;
	#endif
	#ifdef CHIP
	screenScale = 1;
	#elif defined(PYRA)
	//screenScale = 3;
	desktopFS = 1;
	#elif defined(ODROID)
	desktopFS = 1;
	#else
	screenScale = 2;
	#endif
	useJoystick = 1;
	// get command line arguments
	for (int i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "-f"))
			wantFullscreen = 1;
		if(!strcmp(argv[i], "--fullscreen"))
			wantFullscreen = 1;
		if(!strcmp(argv[i], "-d"))
			desktopFS = 1;
		if(!strcmp(argv[i], "--desktop"))
			desktopFS = 1;
		if(!strcmp(argv[i], "-x1"))
			screenScale = 1;
		if(!strcmp(argv[i], "-x2"))
			screenScale = 2;
		if(!strcmp(argv[i], "-x3"))
			screenScale = 3;
		if(!strcmp(argv[i], "-x4"))
			screenScale = 4;
		if(!strcmp(argv[i], "-x5"))
			screenScale = 5;
		if(!strcmp(argv[i], "--xbrz"))
			setXBRZ(1);
		if(!strcmp(argv[i], "--no-xbrz"))
			setXBRZ(0);
		if(!strcmp(argv[i], "-j"))
			useJoystick = 0;
		if(!strcmp(argv[i], "--nojoy"))
			useJoystick = 0;
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			printf("Quick help\n-f|--fullscreen\tUse fullscreen\n-d|--desktop\tdesktop fullscreen\n-x1|-x2|-x3|-x4\tUse screenScale of *1..*4 (default *2 = 640x480)\n-j|-nojoy\tdo not use Joystick\n--xbrz\tUse xBRZ scaling\n--no-xbrz\tNo xBRZ scaling\n");
			exit(0);
		}
	}
	if(desktopFS)
	{
		#ifdef _SDL2
		SDL_DisplayMode infos;
		SDL_GetCurrentDisplayMode(0, &infos);
		screenH = infos.w;
		screenW = infos.h;
		#else
		const SDL_VideoInfo* infos = SDL_GetVideoInfo();
		screenH = infos->current_h;
		screenW = infos->current_w;
		#endif
		if(screenW/320 < screenH/240)
			screenScale = screenW/320;
		else
			screenScale = screenH/240; // automatic guess the scale
		deltaX = (screenW-320*screenScale)/2;
		deltaY = (screenH-240*screenScale)/2;

	} else {
		screenW = 320 * screenScale;
		screenH = 240 * screenScale;
	}
	printf("Hydra Caslte Labyrinth, %s %dx%d scale=x%d%s, using Joystick=%d\n", (wantFullscreen || desktopFS)?"Fullscreen":"Windowed", screenW, screenH, screenScale, getXBRZ()?" xBRZ":"", useJoystick);
	#endif
	
	srand(time(NULL));
	createSaveLocations();
	
	game();

	//System specific cleanup	
	#ifdef _PSP
		sceKernelExitGame();
	#endif
	
	#ifdef _3DS
		sdmcExit();
	#endif

	return 0;
}