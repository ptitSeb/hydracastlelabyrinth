#include "PHL.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

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
		strcpy(buff, getenv("HOME"));
		strcat(buff, "/.hydracastlelabyrinth");
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
	#ifdef PANDORA
	wantFullscreen = 1;
	#else
	wantFullscreen = 0;
	#endif
	screenScale = 2;
	useJoystick = 1;
	// get command line arguments
	for (int i=1; i<argc; i++)
	{
		if(!strcmp(argv[i], "-f"))
			wantFullscreen = 1;
		if(!strcmp(argv[i], "--fullscreen"))
			wantFullscreen = 1;
		if(!strcmp(argv[i], "-x1"))
			screenScale = 1;
		if(!strcmp(argv[i], "-x2"))
			screenScale = 2;
		if(!strcmp(argv[i], "-x3"))
			screenScale = 3;
		if(!strcmp(argv[i], "-x4"))
			screenScale = 4;
		if(!strcmp(argv[i], "-j"))
			useJoystick = 0;
		if(!strcmp(argv[i], "--nojoy"))
			useJoystick = 0;
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			printf("Quick help\n\t-f|--fullscreen\tUse fullscreen\n\t-x1|-x2|-x3|-x4\tUse screenScale of *1..*4 (default *2 = 640x480)\n\t-j|-nojoy\tdo not use Joystick\n");
			exit(0);
		}
	}
	printf("Hydra Caslte Labyrinth, %s scale=x%d, using Joystick=%d\n", wantFullscreen?"Fullscreen":"Windowed", screenScale, useJoystick);
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