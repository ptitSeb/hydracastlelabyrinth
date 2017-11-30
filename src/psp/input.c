#include "input.h"
#include "../text.h"

int deadZone = 64;

void updateKey(Button* btn, int state);

void PHL_ScanInput()
{
	oslReadKeys();
	
	int pUp = 0, pDown = 0, pLeft = 0, pRight = 0;
	
	if (osl_keys->held.up || osl_keys->analogY < -deadZone) { pUp = 1; }
	if (osl_keys->held.down || osl_keys->analogY > deadZone) { pDown = 1; }
	if (osl_keys->held.left || osl_keys->analogX < -deadZone) { pLeft = 1; }
	if (osl_keys->held.right || osl_keys->analogX > deadZone) { pRight = 1; }

	updateKey(&btnUp, pUp);
	updateKey(&btnDown, pDown);
	updateKey(&btnLeft, pLeft);
	updateKey(&btnRight, pRight);
	
	updateKey(&btnL, osl_keys->held.L);
	updateKey(&btnR, osl_keys->held.R);
	
	updateKey(&btnStart, osl_keys->held.start);
	updateKey(&btnSelect, osl_keys->held.select);
	
	updateKey(&btnFaceRight, osl_keys->held.circle);
	updateKey(&btnFaceDown, osl_keys->held.cross);
	updateKey(&btnFaceLeft, osl_keys->held.square);
	updateKey(&btnFaceUp, osl_keys->held.triangle);
	
	btnAccept = btnFaceDown;
	btnDecline = btnFaceRight;
	//Swap buttons for japanese
	if (getLanguage() == JAPANESE) {
		btnAccept = btnFaceRight;
		btnDecline = btnFaceDown;
	}
	
	/*
	if (btnFaceUp.pressed == 1) {
		FILE* f;
		
		if ( (f = fopen("debug.txt", "a")) ) {
			fprintf(f, "\n%i bytes available", oslGetRamStatus().maxAvailable);
		}
		
		fclose(f);
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