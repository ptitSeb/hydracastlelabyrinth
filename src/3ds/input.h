#ifndef INPUT_H
#define INPUT_H

#include <3ds.h>

typedef struct {
	int pressed,
		held,
		released;
} Button;

Button btnUp, btnDown, btnLeft, btnRight;
Button btnFaceUp, btnFaceDown, btnFaceLeft, btnFaceRight;

Button btnL, btnR;
Button btnStart, btnSelect;

Button btnAccept;
Button btnDecline;

int axisX, axisY;

void PHL_ScanInput();

#endif