#pragma once
#include <stdint.h>
#include <stdbool.h>
typedef enum
{
	DisplayRed = 0,
	DisplayGreen,
}Displays;
void InitDisplayTimer();
void SetDisplayNumeric(Displays display, uint8_t val, bool doDisplay);
void SetCharacterDisplay(Displays display, char left, char right, bool doDisplay);