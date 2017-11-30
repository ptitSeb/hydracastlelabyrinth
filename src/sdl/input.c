#include "input.h"
#include <SDL/SDL.h>

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

void Input_KeyEvent(SDL_Event* evt)
{
    int w = (evt->type==SDL_KEYDOWN)?1:0;
    switch(evt->key.keysym.sym)
    {
        case SDLK_UP:       bUp = w; break;
        case SDLK_DOWN:     bDown = w; break;
        case SDLK_LEFT:     bLeft = w; break;
        case SDLK_RIGHT:    bRight = w; break;
#ifdef PANDORA
        case SDLK_PAGEUP:   bFaceUp = w; break;
        case SDLK_PAGEDOWN: bFaceDown = w; break;
        case SDLK_END:      bFaceLeft = w; break;   // reversing, so (B) is sword
        case SDLK_HOME:     bFaceRight = w; break;
        case SDLK_RCTRL:    bR = w; break;
        case SDLK_RSHIFT:   bL = w; break;
        case SDLK_LCTRL:    bSelect = w; break;
        case SDLK_LALT:     bStart = w; break;
#else
        case SDLK_e:        bFaceUp = w; break;
        case SDLK_x:        bFaceDown = w; break;
        case SDLK_s:        bFaceLeft = w; break;
        case SDLK_d:        bFaceRight = w; break;
        case SDLK_r:        bR = w; break;
        case SDLK_w:        bL = w; break;
        case SDLK_SPACE:    bSelect = w; break;
        case SDLK_RETURN:   bStart = w; break;
#endif
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
	updateKey(&btnUp, bUp);
	updateKey(&btnDown, bDown);
	updateKey(&btnLeft, bLeft);
	updateKey(&btnRight, bRight);
	
	updateKey(&btnStart, bStart);
	updateKey(&btnSelect, bSelect);
	
	updateKey(&btnL, bL);
	updateKey(&btnR, bR);
	
	updateKey(&btnFaceLeft, bFaceLeft);
	updateKey(&btnFaceDown, bFaceDown);
	updateKey(&btnFaceRight, bFaceRight);
	
	updateKey(&btnAccept, bFaceLeft);
	updateKey(&btnDecline, bFaceDown);
}
