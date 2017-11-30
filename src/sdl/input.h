#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>

typedef struct {
	int pressed,
		held,
		released;
} Button;

extern Button btnUp, btnDown, btnLeft, btnRight;
extern Button btnFaceUp, btnFaceDown, btnFaceLeft, btnFaceRight;
extern Button btnL, btnR;
extern Button btnStart, btnSelect;
extern Button btnAccept, btnDecline;
extern int axisX, axisY;

void PHL_ScanInput();

#endif