#pragma once
#include "Color.h"
typedef enum
{
	Display_0       = 0,
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
	Display_n,
	Display_O,
	Display_P,
	Display_r,
	Display_S,
	Display_Y,
	Display_t,
	Display_Nothing,
}SegmentVal;

void SetDisplay(SegmentVal v0, Color c0, SegmentVal v1, Color c1);
SegmentVal GetSegmentForInt(uint8_t singleDecimal);