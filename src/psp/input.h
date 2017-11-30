#ifndef INPUT_H
#define INPUT_H

#include <oslib/oslib.h>

typedef struct {
	int pressed,
		held,
		released;
} Button;

Button btnUp, btnDown, btnLeft, btnRight;
Button btnFaceUp, btnFaceDown, btnFaceLeft, btnFaceRight;
Button btnL, btnR;
Button btnStart, btnSelect;
Button btnAccept, btnDecline;
int axisX, axisY;

void PHL_ScanInput();

#endif