#include "Display.h"

#define SEGMENT_BUFFER_SIZE 5
static Color currentColor = { 0 };
static uint8_t segmentBufferCount = 0;
static SegmentVal SegmentBuffer[SEGMENT_BUFFER_SIZE];

void DisplayDeltaText(Color c, uint8_t value)
{
	const SegmentVal v0 = Display_C;
	const SegmentVal v1 = Display_H;
	currentColor = c;
}

void DisplayRemainingText(Color c, uint8_t value)
{
	const SegmentVal v0 = Display_r;
	const SegmentVal v1 = Display_E;
	currentColor = c;
}

void DisplayTotalText(Color c, uint8_t value)
{
	const SegmentVal v0 = Display_P;
	const SegmentVal v1 = Display_t;
	currentColor = c;
}

void DisplayLowVoltage()
{
	const Color c0 = {0x00, 0x00, 0x00};
	const SegmentVal v0 = Display_L;
	const SegmentVal v1 = Display_b;
	currentColor = ColorRed;
}

void DisplayWin(Color c)
{
	const SegmentVal v0 = Display_d;
	const SegmentVal v1 = Display_O;
	currentColor = c;
}


void DisplayValue(Color c, uint8_t value)
{
	//Cutoff anything over 99. None of this 3 digit stuff.
	value = value > 99 ? 99 : value;
	SegmentVal v0 = value / 10;
	SegmentVal v1 = value % 10;
	currentColor = c;
}

void DisplayValues(Color c, SegmentVal* vals, uint8_t length)
{
	
}

void RunDisplayStateMachine()
{
	
}