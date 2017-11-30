#include "input.h"

void updateKey(Button* btn, int state);

void PHL_ScanInput()
{	
	hidScanInput();
	u32 kDown = hidKeysHeld();
		
	updateKey(&btnUp, kDown & KEY_UP);
	updateKey(&btnDown, kDown & KEY_DOWN);
	updateKey(&btnLeft, kDown & KEY_LEFT);
	updateKey(&btnRight, kDown & KEY_RIGHT);
	
	updateKey(&btnStart, kDown & KEY_START);
	updateKey(&btnSelect, kDown & KEY_SELECT);
	
	updateKey(&btnFaceRight, kDown & KEY_A);
	updateKey(&btnFaceDown, kDown & KEY_B);
	updateKey(&btnFaceLeft, kDown & KEY_Y);
	
	updateKey(&btnL, kDown & KEY_L);
	updateKey(&btnR, kDown & KEY_R);
	
	updateKey(&btnAccept, kDown & KEY_A);
	updateKey(&btnDecline, kDown & KEY_B);
	/*
	//Start button
	if (kDown & KEY_START) {
		btnStart.pressed = 1;
	}else{
		btnStart.pressed = 0;
	}
	*/
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