#include "Display.h"
#include "Segment.h"

void DisplayDeltaText(Color c)
{
	const SegmentVal v0 = Display_C;
	const SegmentVal v1 = Display_H;
	SetDisplay(v0, c, v1, c);
}

void DisplayRemainingText(Color c)
{
	const SegmentVal v0 = Display_r;
	const SegmentVal v1 = Display_E;
	SetDisplay(v0, c, v1, c);
}

void DisplayTotalText(Color c)
{
	const SegmentVal v0 = Display_P;
	const SegmentVal v1 = Display_t;
	SetDisplay(v0, c, v1, c);
}

void DisplayLowVoltage()
{
	const Color c0 = {0x00, 0x00, 0x00};
	const SegmentVal v0 = Display_L;
	const SegmentVal v1 = Display_b;
	SetDisplay(v0, c0, v1, c0);
}

void DisplayWin1(Color c)
{
	const SegmentVal v0 = Display_d;
	const SegmentVal v1 = Display_O;
	SetDisplay(v0, c, v1, c);
}

void DisplayWin2(Color c)
{
	const SegmentVal v0 = Display_O;
	const SegmentVal v1 = Display_n;
	SetDisplay(v0, c, v1, c);
}

void DisplayWin3(Color c)
{
	const SegmentVal v0 = Display_n;
	const SegmentVal v1 = Display_E;
	SetDisplay(v0, c, v1, c);
}

void DisplayValue(Color c, uint8_t value)
{
	//Cutoff anything over 99. None of this 3 digit stuff.
	value = value > 99 ? 99 : value;
	SegmentVal v0 = value / 10;
	SegmentVal v1 = value % 10;
	SetDisplay(v0, c, v1, c);
}