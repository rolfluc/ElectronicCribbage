#include "AddressableLED.h"
#include <memory.h>
#include "LEDTiming.h"


void byteToBuffer(colorBuffer* buffer, uint8_t colorByte)
{
	static const uint16_t bitHigh = BIT_HIGH_COUNTS;
	static const uint16_t bitLow = BIT_LOW_COUNTS;
	// Preserve endianess
	for (int8_t i = 7; i >= 0; i--)
	{
		buffer->buffer[i] = (colorByte & (1 << i)) == 1 ? bitHigh : bitLow;
	}
}

void FillColor(PaddedColor* buffer, Color color)
{
	byteToBuffer(&(buffer->R), color.R);
	byteToBuffer(&(buffer->G), color.G);
	byteToBuffer(&(buffer->B), color.B);
#ifdef RGBW
	byteToBuffer(&(buffer->W), color.W);
#endif // RGBW
}