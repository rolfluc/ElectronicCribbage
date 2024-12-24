#include "Segment.h"
#include "AddressableLED.h"
//#include "SPI.h"
#include "PWM.h"
#include <memory.h>
// Total Data size, 2 x 7 segments, 3 colors per segment, 4 bits of data per bit of color
// 24 bits of color, 24 * 4 (bits of SPI data) / 8 = 12 bytes
#define NUMBER_SEGMENTS 7 * 2
#define BYTES_PER_SEGMENT 12

// 7 Segment Data ordering is:
// D1->E1->F1->A1->B1->G1->C1->E2->F2->A2->B2->C2->G2->D2

typedef struct 
{
	PaddedColor D;
	PaddedColor E;
	PaddedColor F;
	PaddedColor A;
	PaddedColor B;
	PaddedColor G;
	PaddedColor C;
}Segment0;

typedef struct 
{
	PaddedColor E;
	PaddedColor F;
	PaddedColor A;
	PaddedColor B;
	PaddedColor C;
	PaddedColor G;
	PaddedColor D;
}Segment1;

typedef struct 
{
	Segment0 seg0;
	Segment1 seg1;
}Display;

Display displayBuffer;

static inline void ColorToBuffer(Color c, PaddedColor* pc)
{
	FillColor(pc, c);
}

static inline void SetDark(PaddedColor* c)
{
	static const Color ColorDark = { 0, 0, 0 };
	ColorToBuffer(ColorDark, c);
}

static inline void FillBuffer(SegmentVal v, Color col, PaddedColor* a, PaddedColor* b, PaddedColor* c, PaddedColor* d, PaddedColor* e, PaddedColor* f, PaddedColor* g)
{
	switch (v)
	{
	case Display_O:
	case Display_0:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			SetDark(g);
			break;
		}
	case Display_1:
		{
			SetDark(a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			SetDark(d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
			break;
		}
	case Display_2:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			SetDark(c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			SetDark(f);
			ColorToBuffer(col, g);
			break;                                 
		}
	case Display_3:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			SetDark(e);
			SetDark(f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_4:
		{
			SetDark(a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			SetDark(d);
			SetDark(e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_S:
	case Display_5:
		{
			ColorToBuffer(col, a);
			SetDark(b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			SetDark(e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_6:
		{
			ColorToBuffer(col, a);
			SetDark(b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_7:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			SetDark(d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
			break;
		}
	case Display_8:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_9:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			SetDark(d);
			SetDark(e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_A:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			SetDark(d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_b:
		{
			SetDark(a);
			SetDark(b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_C:
		{
			ColorToBuffer(col, a);
			SetDark(b);
			SetDark(c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			SetDark(g);
			break;
		}
	case Display_d:
		{
			SetDark(a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			SetDark(f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_E:
		{
			ColorToBuffer(col, a);
			SetDark(b);
			SetDark(c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_F:
		{
			ColorToBuffer(col, a);
			SetDark(b);
			SetDark(c);
			SetDark(d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_H:
		{
			SetDark(a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			SetDark(d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_L:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			SetDark(g);
			break;
		}
	case Display_n:
		{
			SetDark(a);
			SetDark(b);
			ColorToBuffer(col, c);
			SetDark(d);
			ColorToBuffer(col, e);
			SetDark(f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_P:
		{
			ColorToBuffer(col, a);
			ColorToBuffer(col, b);
			SetDark(c);
			SetDark(d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_r:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			SetDark(d);
			ColorToBuffer(col, e);
			SetDark(f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_t:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			ColorToBuffer(col, d);
			ColorToBuffer(col, e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_Y:
		{
			SetDark(a);
			ColorToBuffer(col, b);
			ColorToBuffer(col, c);
			ColorToBuffer(col, d);
			SetDark(e);
			ColorToBuffer(col, f);
			ColorToBuffer(col, g);
			break;
		}
	case Display_Nothing:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			SetDark(d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
			break;
		}
	}
}

void SetDisplay(SegmentVal v0, Color c0, SegmentVal v1, Color c1)
{
	// Kind of gross way to do this. But hey. It works.
	FillBuffer(v0, c0, &displayBuffer.seg0.A, &displayBuffer.seg0.B, &displayBuffer.seg0.C, &displayBuffer.seg0.D, &displayBuffer.seg0.E, &displayBuffer.seg0.F, &displayBuffer.seg0.G);
	FillBuffer(v1, c1, &displayBuffer.seg1.A, &displayBuffer.seg1.B, &displayBuffer.seg1.C, &displayBuffer.seg1.D, &displayBuffer.seg1.E, &displayBuffer.seg1.F, &displayBuffer.seg1.G);
	
	SendPWMDMA((uint8_t*)&displayBuffer, sizeof(Display) / sizeof(uint16_t));
	//SendTimerDMA((uint8_t*)&displayBuffer, sizeof(Display));
}

SegmentVal GetSegmentForInt(uint8_t singleDecimal)
{
	if (singleDecimal > 9)
	{
		return Display_9;
	}
	return (SegmentVal)singleDecimal;
}