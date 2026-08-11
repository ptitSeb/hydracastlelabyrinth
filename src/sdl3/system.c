#include <SDL3/SDL.h>
#include <stdio.h>
#include "system.h"


char quitGame = 0;

void Input_KeyEvent(SDL_Event* evt);
void Input_JoyEvent(SDL_Event* evt);
void Input_JoyAxisEvent(SDL_Event* evt);
void Input_JoyHatEvent(SDL_Event* evt);
void Input_ControlButtonEvent(SDL_Event* evt);
void Input_ControlAxisEvent(SDL_Event* evt);

int PHL_MainLoop()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
            case SDL_EVENT_QUIT:
                quitGame = 1;
                return 0;
            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
                Input_KeyEvent(&evt);
                break;
            case SDL_EVENT_JOYSTICK_AXIS_MOTION:
                Input_JoyAxisEvent(&evt);
                break;
            case SDL_EVENT_JOYSTICK_HAT_MOTION:
                Input_JoyHatEvent(&evt);
                break;
            case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
            case SDL_EVENT_JOYSTICK_BUTTON_UP:
                Input_JoyEvent(&evt);
                break;
            case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                Input_ControlAxisEvent(&evt);
                break;
            case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            case SDL_EVENT_GAMEPAD_BUTTON_UP:
                Input_ControlButtonEvent(&evt);
                break;
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
