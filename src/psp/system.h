#ifndef SYSTEM_H
#define SYSTEM_H

#include <pspkernel.h>
#include <pspdebug.h>
#include "../PHL.h"

char quitGame;

int PHL_MainLoop();
void PHL_ConsoleInit();
void PHL_GameQuit();

void PHL_ErrorScreen(char* message);

#endif