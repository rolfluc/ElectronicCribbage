#pragma once
#include <stdint.h>
#include <stdbool.h>
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

static bool areColorsTheSame(Color c1, Color c2) {
	return (c1.B == c2.B) && (c1.R == c2.R) && (c1.G == c2.G);
}

static const Color ColorRed = { .R = 0x7f, .G = 0, .B = 0 };
static const Color ColorGreen = { .R = 0, .G = 0x7f, .B = 0 };
static const Color ColorBlue = { .R = 0, .G = 0, .B = 0x7f };
static const Color ColorWhite = { .R = 0xff, .G = 0xff, .B = 0xff };
static const Color ColorUndefined = {.R = 0x12, .G = 0x34, .B = 0x56 }; // Kind of gross. But simply to indicate use current Color