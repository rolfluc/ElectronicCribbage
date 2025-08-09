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
	case Display_underscore:
		{
			SetDark(a);
			SetDark(b);
			SetDark(c);
			ColorToBuffer(col, d);
			SetDark(e);
			SetDark(f);
			SetDark(g);
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

SegmentVal GetSegmentForChar(char val) {
	switch(val)
	{
	case '0':
		return Display_0;
	case '1':
		return Display_1;
	case '2':
		return Display_2;
	case '3':
		return Display_3;
	case '4':
		return Display_4;
	case 'S': // intentional fallthrough
	case 's': // intentional fallthrough
	case '5':
		return Display_5;
	case '6':
		return Display_6;
	case '7':
		return Display_7;
	case '8':
		return Display_8;
	case '9':
		return Display_9;
	case 'A':
	case 'a': // intentional fallthrough
		return Display_A;
	case 'B': // intentional fallthrough
	case 'b':
		return Display_b;
	case 'C': // intentional fallthrough
	case 'c':
	 	return Display_C;
	case 'D': // intentional fallthrough
	case 'd':
		return Display_d;
	case 'E': // intentional fallthrough
	case 'e':
		return Display_E;
	case 'F': // intentional fallthrough
	case 'f': 
		return Display_F;
	case 'H': // intentional fallthrough
	case 'h':
		return Display_H;
	case 'L': // intentional fallthrough
	case 'l':
		return Display_L;
	case 'N': // intentional fallthrough
	case 'n':
		return Display_n;
	case 'O': // intentional fallthrough
	case 'o':
		return Display_o;
	case 'P': // intentional fallthrough
	case 'p':
		return Display_P;
	case 'R': // intentional fallthrough
	case 'r':
		return Display_r;
	case 'Y': // intentional fallthrough
	case 'y':
		return Display_Y;
	case 'T': // intentional fallthrough
	case '_':
		return Display_underscore;
	case '-': // Intentional fallthrough.  Unknown characters become dashes
	default:
		return Display_dash;
	}
}

char GetCharFromSegmentVal(SegmentVal val)
{
	switch(val)
	{
	case Display_0:
		return '0';
	case Display_1:
		return '1';
	case Display_2:
		return '2';
	case Display_3:
		return '3';
	case Display_4:
		return '4';
	case Display_5:
		return '5';
	case Display_6:
		return '6';
	case Display_7:
		return '7';
	case Display_8:
		return '8';
	case Display_9:
		return '9';
	case Display_A:
		return 'a';
	case Display_b:
		return 'b';
	case Display_C:
	 	return 'c';
	case Display_d:
		return 'd';
	case Display_E:
		return 'E';
	case Display_F:
		return 'F';
	case Display_H:
		return 'H';
	case Display_L:
		return 'L';
	case Display_n:
		return 'n';
	case Display_o:
		return 'o';
	case Display_P:
		return 'P';
	case Display_r:
		return 'r';
	case Display_Y:
		return 'y';
	case Display_underscore:
		return '_';
	case Display_dash:
		return '-';
	default:
		return '-';
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
