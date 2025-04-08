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

static const Color ColorRed = { .R = 0x7f, .G = 0, .B = 0 };
static const Color ColorGreen = { .R = 0, .G = 0x7f, .B = 0 };
static const Color ColorBlue = { .R = 0, .G = 0, .B = 0x7f };