#pragma once
#include "PinDefs.h"
#include <stdbool.h>
typedef struct
{
	const PinDef* A;
	const PinDef* B;
	const PinDef* C;
	const PinDef* D;
	const PinDef* E;
	const PinDef* F;
	const PinDef* G;
	const PinDef* DP;
	const PinDef* Enable;
	bool IsCommonCathode;
}Segment;

typedef enum
{
	Display_0 = 0,
	Display_1,
	Display_2,
	Display_3,
	Display_4,
	Display_5,
	Display_6,
	Display_7,
	Display_8,
	Display_9,
	Display_A,
	Display_b,
	Display_C,
	Display_d,
	Display_E,
	Display_F,
	Display_H,
	Display_L,
	Display_O,
	Display_P,
	Display_S,
	Display_Y,
	Display_Nothing,
}DisplayStates;

void DisplayOnSegment(Segment* sevenSegment, DisplayStates display, bool doShowDP);
void TurnOffSegment(Segment* sevenSegment);

static bool checkValidChar(char chr)
{
	switch (chr)
	{
	case 'A':
	case 'b':
	case 'C':
	case 'd':
	case 'E':
	case 'F':
	case 'H':
	case 'L':
	case 'O':
	case 'P':
	case 'S':
	case 'Y':
		return true;
	default:
		return false;
	}
}