#pragma once
#include "PinDefs.h"
#include <stdbool.h>
typedef struct
{
	PinDef* A;
	PinDef* B;
	PinDef* C;
	PinDef* D;
	PinDef* E;
	PinDef* F;
	PinDef* G;
	PinDef* DP;
	PinDef* Enable;
	bool IsCommonCathode;
}Segment;

typedef enum
{
	Display_0,
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
}DisplayStates;

void DisplayOnSegment(Segment* sevenSegment, DisplayStates display, bool doShowDP);
void TurnOffSegment(Segment* sevenSegment);