#include <SDL/SDL.h>
#include "system.h"


char quitGame = 0;

void Input_KeyEvent(SDL_Event* evt);

int PHL_MainLoop()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
            case SDL_QUIT:
                return 0;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Input_KeyEvent(&evt);
                break;
            //TODO: joystick...
        }
    }
    if (quitGame == 1) 
    {
		return 0;
	}
	return 1;
}
void PHL_ConsoleInit()
{

}
void PHL_GameQuit()
{
    quitGame = 1;
}

void PHL_ErrorScreen(char* message)
{
    fprintf(stderr, "%s\n", message);
}
