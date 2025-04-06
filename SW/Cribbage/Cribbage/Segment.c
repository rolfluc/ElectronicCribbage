#include "Segment.h"
#include "AddressableLED.h"
#include "SPI.h"
//#include "Timer.h"
#include <memory.h>
// Total Data size, 2 x 7 segments, 3 colors per segment, 4 bits of data per bit of color
// 24 bits of color, 24 * 4 (bits of SPI data) / 8 = 12 bytes
// 7 Segment Data ordering is:
// F1->G1->A1->B1->F2->A2->G2->B2->C2->D2->E2->C1->D1->E1->

typedef struct
{
	PaddedColor F1;
	PaddedColor G1;
	PaddedColor A1;
	PaddedColor B1;
	PaddedColor F2;
	PaddedColor A2;
	PaddedColor G2;
	PaddedColor B2;
	PaddedColor C2;
	PaddedColor D2;
	PaddedColor E2;
	PaddedColor C1;
	PaddedColor D1;
	PaddedColor E1;
}Segments;
	

typedef struct 
{
	uint32_t StartOfFrame; 
	Segments seg;
	uint32_t EndOfFrame;
}Display;

Display displayBuffer;

static inline void ColorToBuffer(Color c, PaddedColor* pc)
{
	const uint8_t dimValue = 0x07;
	FillColor(pc, c, dimValue);
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
	displayBuffer.StartOfFrame = 0x00000000;
	displayBuffer.EndOfFrame = 0xFFFFFFFF;
	FillBuffer(v0, c0, &displayBuffer.seg.A1, &displayBuffer.seg.B1, &displayBuffer.seg.C1, &displayBuffer.seg.D1, &displayBuffer.seg.E1, &displayBuffer.seg.F1, &displayBuffer.seg.G1);
	FillBuffer(v1, c1, &displayBuffer.seg.A2, &displayBuffer.seg.B2, &displayBuffer.seg.C2, &displayBuffer.seg.D2, &displayBuffer.seg.E2, &displayBuffer.seg.F2, &displayBuffer.seg.G2);
	
	SendDisplay((uint8_t*) &displayBuffer, sizeof(displayBuffer));
}

SegmentVal GetSegmentForInt(uint8_t singleDecimal)
{
	if (singleDecimal > 9)
	{
		return Display_9;
	}
	return (SegmentVal)singleDecimal;
}