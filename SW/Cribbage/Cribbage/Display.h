#pragma once
#include "stdint.h"
#include "Color.h"
void DisplayDeltaText(Color c);
void DisplayRemainingText(Color c);
void DisplayTotalText(Color c);
void DisplayValue(Color c, uint8_t value);
void DisplayLowVoltage();
void DisplayWin1(Color c);
void DisplayWin2(Color c);
void DisplayWin3(Color c);