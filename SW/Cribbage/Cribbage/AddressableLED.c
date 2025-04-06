#include "AddressableLED.h"
#include <memory.h>
#include "LEDTiming.h"


static inline void byteToBuffer(colorBuffer* buffer, uint8_t colorByte)
{
	buffer->buffer = colorByte;
}

void FillColor(PaddedColor* buffer, Color color, uint8_t dimValue)
{
	const uint8_t FLAG_ON = 0x07; 
	Command cmd = { .Dimming = dimValue, .flags = FLAG_ON };
	buffer->cmd = cmd;
	byteToBuffer(&(buffer->R), color.R);
	byteToBuffer(&(buffer->G), color.G);
	byteToBuffer(&(buffer->B), color.B);
}