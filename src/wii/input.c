//Wii
#include "input.h"
#include <math.h>
#include <wiiuse/wpad.h>

double deadZone = 0.5;

void updateKey(Button* btn, int state);

void PHL_ScanInput()
{
	//btnStartPrev = btnStart;
	
	WPAD_ScanPads();
	//PAD_ScanPads();
	
	u32 pressed = WPAD_ButtonsHeld(0);
	//u32 gPressed = PAD_ButtonsHeld(0);
	
	int pUp = 0, pDown = 0, pLeft = 0, pRight = 0;
	int pFaceLeft = 0, pFaceRight = 0, /*pFaceUp = 0,*/ pFaceDown = 0;
	int pL = 0, pR = 0;
	int pSelect = 0, pStart = 0;
	int pAccept = 0, pDecline = 0;
	
	//if (pressed & WPAD_BUTTON_RIGHT || pressed & WPAD_CLASSIC_BUTTON_UP || gPressed & PAD_BUTTON_RIGHT) { pUp = 1; }
	WPADData* cData = WPAD_Data(0);
	
	if (cData->exp.type == WPAD_EXP_NONE)
	{ //Normal Wiimote*/
		pUp = pressed & WPAD_BUTTON_RIGHT;
		pDown = pressed & WPAD_BUTTON_LEFT;
		pLeft = pressed & WPAD_BUTTON_UP;
		pRight = pressed & WPAD_BUTTON_DOWN;
		
		pSelect = pressed & WPAD_BUTTON_MINUS;
		pStart = pressed & WPAD_BUTTON_PLUS;
		
		pR = pressed & WPAD_BUTTON_A;
		
		pFaceLeft = pressed & WPAD_BUTTON_1;
		pFaceDown = pressed & WPAD_BUTTON_2;
		pFaceRight = pressed & WPAD_BUTTON_B;
		
		pAccept = pFaceDown;
		pDecline = pFaceLeft;
	}else if (cData->exp.type == WPAD_EXP_CLASSIC)
	{ //Classic Controller
		//Get x and y axis from the joystick
		//Taken from the Wii SDL source
		float mag = 0.0,
			  ang = 0.0;
			  
		mag = cData->exp.classic.ljs.mag;
		ang = cData->exp.classic.ljs.ang;
		
		double xAxis = mag * sin((3.14159 * ang)/180.0f);
		double yAxis = mag * cos((3.14159 * ang)/180.0f);
		
		if (yAxis < -deadZone || pressed & WPAD_CLASSIC_BUTTON_DOWN) { pDown = 1; }
		if (yAxis > deadZone || pressed & WPAD_CLASSIC_BUTTON_UP) { pUp = 1; }
		if (xAxis < -deadZone || pressed & WPAD_CLASSIC_BUTTON_LEFT) { pLeft = 1; }
		if (xAxis > deadZone || pressed & WPAD_CLASSIC_BUTTON_RIGHT) { pRight = 1; }
		
		if (pUp == 1 && pDown == 1) {
			pUp = 0;
			pDown = 0;
		}
		if (pLeft == 1 && pRight == 1) {
			pLeft = 0;
			pRight = 0;
		}
		
		pSelect = pressed & WPAD_CLASSIC_BUTTON_MINUS;
		pStart = pressed & WPAD_CLASSIC_BUTTON_PLUS;
		
		pL = pressed & WPAD_CLASSIC_BUTTON_FULL_L;
		pR = pressed & WPAD_CLASSIC_BUTTON_FULL_R;
		
		pFaceLeft = pressed & WPAD_CLASSIC_BUTTON_Y;
		pFaceDown = pressed & WPAD_CLASSIC_BUTTON_B;
		pFaceRight = pressed & WPAD_CLASSIC_BUTTON_A;
		
		pAccept = pFaceRight;
		pDecline = pFaceDown;
	}
	
	//Gamecube Controller
	{
		PAD_ScanPads();
		pressed = PAD_ButtonsHeld(0);
		
		double xAxis = PAD_StickX(0) / 25;
		double yAxis = PAD_StickY(0) / 25;
		
		if (pressed & PAD_BUTTON_UP || yAxis > deadZone) { pUp = 1; }
		if (pressed & PAD_BUTTON_DOWN || yAxis < -deadZone) { pDown = 1; }
		if (pressed & PAD_BUTTON_LEFT || xAxis < -deadZone) { pLeft = 1; }
		if (pressed & PAD_BUTTON_RIGHT || xAxis > deadZone) { pRight = 1; }
		
		if (pUp == 1 && pDown == 1) {
			pUp = 0;
			pDown = 0;
		}
		if (pLeft == 1 && pRight == 1) {
			pLeft = 0;
			pRight = 0;
		}
		
		if (pressed & PAD_BUTTON_START) { pStart = 1; }
		if (pressed & PAD_BUTTON_X) { pSelect = 1; }
		
		if (pressed & PAD_BUTTON_B) { pFaceLeft = 1; }
		if (pressed & PAD_BUTTON_A) { pFaceDown = 1; }
		if (pressed & PAD_BUTTON_Y) { pFaceRight = 1; }
		
		if (pressed & PAD_TRIGGER_L) { pL = 1; }
		if (pressed & PAD_TRIGGER_R) { pR = 1; }
		
		pAccept = pFaceDown;
		pDecline = pFaceLeft;
	}
	
	updateKey(&btnUp, pUp);
	updateKey(&btnDown, pDown);
	updateKey(&btnLeft, pLeft);
	updateKey(&btnRight, pRight);
	
	updateKey(&btnStart, pStart);
	updateKey(&btnSelect, pSelect);
	
	updateKey(&btnL, pL);
	updateKey(&btnR, pR);
	
	updateKey(&btnFaceLeft, pFaceLeft);
	updateKey(&btnFaceDown, pFaceDown);
	updateKey(&btnFaceRight, pFaceRight);
	
	updateKey(&btnAccept, pAccept);
	updateKey(&btnDecline, pDecline);
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