#pragma once
#include "stdint.h"
#include "Color.h"
#include "Segment.h"
void DisplayDeltaText(Color c, uint8_t value);
void DisplayRemainingText(Color c, uint8_t value);
void DisplayTotalText(Color c, uint8_t value);
void DisplayValue(Color c, uint8_t value);
void DisplayValues(Color c, SegmentVal* vals, uint8_t length);
void DisplayLowVoltage();
void DisplayWin(Color c);
void RunDisplayStateMachine();
