#include "input.h"
#ifdef EMSCRIPTEN
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

Button btnUp = {0}, btnDown = {0}, btnLeft = {0}, btnRight = {0};
Button btnFaceUp = {0}, btnFaceDown = {0}, btnFaceLeft = {0}, btnFaceRight = {0};
Button btnL = {0}, btnR = {0};
Button btnStart = {0}, btnSelect = {0};
Button btnAccept = {0}, btnDecline = {0};
int axisX = 0, axisY = 0;

int bUp = 0, bDown = 0, bLeft = 0, bRight = 0;
int bFaceUp = 0, bFaceDown = 0, bFaceLeft = 0, bFaceRight = 0;
int bR = 0, bL = 0;
int bStart = 0, bSelect = 0;
int bAccept = 0, bDecline = 0;
int jUp = 0, jDown = 0, jLeft = 0, jRight = 0;
int jFaceUp = 0, jFaceDown = 0, jFaceLeft = 0, jFaceRight = 0;
int jR = 0, jL = 0;
int jStart = 0, jSelect = 0;
int jAccept = 0, jDecline = 0;

SDL_Joystick *joy1 = NULL;
SDL_GameController *controller = NULL;

int useJoystick = 1;

void Input_InitJoystick()
{
	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
	int n = SDL_NumJoysticks();
	if (n) {
		if(SDL_IsGameController(0)) {
			controller = SDL_GameControllerOpen(0);
			SDL_GameControllerEventState(SDL_ENABLE);
			const char *name = SDL_GameControllerNameForIndex(0);
			printf("Using controller %s\n", name?name:"with no name");
		} else {
			joy1 = SDL_JoystickOpen(0);
			SDL_JoystickEventState(SDL_ENABLE);
			printf("Using %s\n", SDL_JoystickName(0));
		}
	} else {
		joy1 = NULL;
		controller = NULL;
	}
}

void Input_CloseJoystick()
{
	if(joy1)
		SDL_JoystickClose(joy1);
	joy1 = NULL;
	if(controller)
		SDL_GameControllerClose(controller);
	controller = NULL;
}

void Input_KeyEvent(SDL_Event* evt)
{
    int w = (evt->type==SDL_KEYDOWN)?1:0;
    switch(evt->key.keysym.sym)
    {
        case SDLK_UP:       bUp = w; break;
        case SDLK_DOWN:     bDown = w; break;
        case SDLK_LEFT:     bLeft = w; break;
        case SDLK_RIGHT:    bRight = w; break;
#if defined(PANDORA) || defined(PYRA)
        case SDLK_PAGEUP:   bFaceUp = w; break;
        case SDLK_PAGEDOWN: bFaceDown = w; break;
        case SDLK_END:      bFaceLeft = w; break;   // reversing, so (B) is sword
        case SDLK_HOME:     bFaceRight = w; break;
        case SDLK_RCTRL:    bR = w; break;
        case SDLK_RSHIFT:   bL = w; break;
        case SDLK_LCTRL:    bSelect = w; break;
		case SDLK_LALT:     bStart = w; break;
#elif defined(CHIP)
		case SDLK_MINUS:        bFaceUp = w; break;
		case SDLK_o:            bFaceDown = w; break;
		case SDLK_0:            bFaceLeft = w; break;
		case SDLK_EQUALS:       bFaceRight = w; break;
		case SDLK_1:            bR = w; break;
		case SDLK_2:            bL = w; break;
		case SDLK_SPACE:        bSelect = w; break;
		case SDLK_RETURN:       bStart = w; break;
#else
        case SDLK_e:        bFaceUp = w; break;
        case SDLK_x:        bFaceDown = w; break;
        case SDLK_s:        bFaceLeft = w; break;
        case SDLK_d:        bFaceRight = w; break;
        case SDLK_r:        bR = w; break;
        case SDLK_w:        bL = w; break;
		case SDLK_SPACE:    bSelect = w; break;
		case SDLK_ESCAPE:   bSelect = w; break;
        case SDLK_RETURN:   bStart = w; break;
#endif
    }
}

void Input_JoyAxisEvent(SDL_Event* evt) {
	if(controller)
		return;
	if(evt->jaxis.which!=0)
		return;
	#define DEADZONE 32
	if(evt->jaxis.axis==0) {
		int v = (evt->jaxis.value)/256;
		if(v>-DEADZONE & v<DEADZONE) axisX = 0;
		else if(v<0) axisX = -1;
		else axisX = +1;
	}
	if(evt->jaxis.axis==1) {
		int v = (evt->jaxis.value)/256;
		if(v>-DEADZONE & v<DEADZONE) axisY = 0;
		else if(v<0) axisY = -1;
		else axisY = +1;
	}
}

void Input_JoyEvent(SDL_Event* evt) {
	if(controller)
		return;
	if(evt->jbutton.which!=0)
		return;
	int w = (evt->type==SDL_JOYBUTTONDOWN)?1:0;
	switch(evt->jbutton.button)
	{
		case  0: jFaceDown = w; break;
		case  1: jFaceLeft = w; break;
		case  2: jFaceRight = w; break;
		case  3: jFaceUp = w; break;
		case  4: jL = w; break;
		case  5: jR = w; break;
		case  6: jSelect = w; break;
		case  7: jStart = w; break;
	}
}

void Input_JoyHatEvent(SDL_Event* evt) {
	if(controller)
		return;
	if(evt->jhat.which!=0)
		return;
	if(evt->jhat.hat!=0)
		return;
	int v=evt->jhat.value;
	jUp = v&SDL_HAT_UP;
	jDown = v&SDL_HAT_DOWN;
	jLeft = v&SDL_HAT_LEFT;
	jRight = v&SDL_HAT_RIGHT;
}

void Input_ControlButtonEvent(SDL_Event* evt) {
	if(evt->cbutton.which!=0)
		return;
	int w = (evt->cbutton.state==SDL_PRESSED)?1:0;
	switch(evt->cbutton.button)
	{
		case SDL_CONTROLLER_BUTTON_A:	jFaceDown = w; break;
		case SDL_CONTROLLER_BUTTON_B: 	jFaceLeft = w; break;
		case SDL_CONTROLLER_BUTTON_X: 	jFaceRight = w; break;
		case SDL_CONTROLLER_BUTTON_Y: 	jFaceUp = w; break;
		case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: jL = w; break;
		case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: jR = w; break;
		case SDL_CONTROLLER_BUTTON_BACK: jSelect = w; break;
		case SDL_CONTROLLER_BUTTON_START: jStart = w; break;
		case SDL_CONTROLLER_BUTTON_DPAD_UP: jUp = w; break;
		case SDL_CONTROLLER_BUTTON_DPAD_DOWN: jDown = w; break;
		case SDL_CONTROLLER_BUTTON_DPAD_LEFT: jLeft = w; break;
		case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: jRight = w; break;
	}
}
#ifdef __amigaos4__
#define STICK_X SDL_CONTROLLER_AXIS_RIGHTX
#define STICK_Y SDL_CONTROLLER_AXIS_RIGHTY
#else
#define STICK_X SDL_CONTROLLER_AXIS_LEFTX
#define STICK_Y SDL_CONTROLLER_AXIS_LEFTY
#endif
void Input_ControlAxisEvent(SDL_Event* evt) {
	if(evt->caxis.which!=0)
		return;
	#define DEADZONE 32
	if(evt->caxis.axis==STICK_X) {
		int v = (evt->caxis.value)/256;
		if(v>-DEADZONE & v<DEADZONE) axisX = 0;
		else if(v<0) axisX = -1;
		else axisX = +1;
	} else if(evt->caxis.axis==STICK_Y) {
		int v = (evt->caxis.value)/256;
		if(v>-DEADZONE & v<DEADZONE) axisY = 0;
		else if(v<0) axisY = -1;
		else axisY = +1;
	}
}

void updateKey(Button* btn, int state)
{
	if (state) {
		if (btn->held == 1) {
			btn->pressed = 0;
		}else{
			btn->pressed = 1;
		}
		btn->held = 1;
		btn->released = 0;
	}else{
		if (btn->held == 1) {
			btn->released = 1;
		}else{
			btn->released = 0;
		}
		btn->held = 0;
		btn->pressed = 0;
	}
}

void PHL_ScanInput()
{
	updateKey(&btnUp, bUp|jUp|(axisY<0));
	updateKey(&btnDown, bDown|jDown|(axisY>0));
	updateKey(&btnLeft, bLeft|jLeft|(axisX<0));
	updateKey(&btnRight, bRight|jRight|(axisX>0));
	
	updateKey(&btnStart, bStart|jStart);
	updateKey(&btnSelect, bSelect|jSelect);
	
	updateKey(&btnL, bL|jL);
	updateKey(&btnR, bR|jR);
	
	updateKey(&btnFaceLeft, bFaceLeft|jFaceLeft);
	updateKey(&btnFaceDown, bFaceDown|jFaceDown);
	updateKey(&btnFaceRight, bFaceRight|jFaceRight);
	
	updateKey(&btnAccept, bFaceLeft|jFaceLeft);
	updateKey(&btnDecline, bFaceDown|jFaceDown);
}
