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