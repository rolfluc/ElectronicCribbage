#pragma once
#include <stdint.h>
#include "Color.h"

// Need 1 half word, per color bit.
// 1 LED Color is 24 half words, so 48 bytes.
#define BUFFER_SIZE 8
typedef struct 
{
	uint16_t buffer[BUFFER_SIZE];
	//uint8_t buffer[BUFFER_SIZE];
}colorBuffer;

typedef struct 
{
	colorBuffer R;
	colorBuffer G;
	colorBuffer B;
}PaddedColor;

// Places the respective color at the prescribed location. Assumes the buffer has been allocated.
void FillColor(PaddedColor* buffer, Color color);