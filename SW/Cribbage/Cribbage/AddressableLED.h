#pragma once
#include <stdint.h>
#include "Color.h"

// Dialight 587 Structure:

#define CMD_NORMAL 0x07
#define CMD_SLEEP 0x05

typedef union
{
	struct
	{
		uint8_t Dimming : 5;	
		uint8_t flags : 3;
	};
	uint8_t raw;
}Command;

typedef struct 
{
	uint8_t buffer;
}colorBuffer;

typedef struct 
{
	Command cmd;
	colorBuffer B;
	colorBuffer G;
	colorBuffer R;
}PaddedColor;

// Places the respective color at the prescribed location. Assumes the buffer has been allocated.
void FillColor(PaddedColor* buffer, Color color, uint8_t dimValue);