#pragma once
#include <stdint.h>
#define RGB
#ifdef RGB
#undef RGBW
#else
#define RGBW
#endif
typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
#ifdef RGBW
	uint8_t w;
#endif
}Color;