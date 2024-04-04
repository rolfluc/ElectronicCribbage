#include "Display.h"
#include "SevenSegment.h"

#define NUMBER_SEGMENTS 4
#define NUMBER_DISPLAYS NUMBER_SEGMENTS/2

typedef struct 
{
	bool doDisplay;
	DisplayStates left;
	DisplayStates right;
	Segment* leftSegment;
	Segment* rightSegment;
}DisplayVals;

static Segment Segments[NUMBER_SEGMENTS] = { 
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D1_R_EN, true},
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D2_R_EN, true},
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D1_G_EN, true},
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D2_G_EN, true},
};

static DisplayVals SegmentDisplays[NUMBER_DISPLAYS] = { 
	{ false, Display_Nothing, Display_Nothing, &Segments[0], &Segments[1] },
	{ false, Display_Nothing, Display_Nothing, &Segments[2], &Segments[3] },
};

static uint8_t SegmentTarget = 0;

static inline void DisplayTimerInterrupt()
{
	// Turn off the old Segment
	TurnOffSegment(&Segments[SegmentTarget]);
	SegmentTarget = (SegmentTarget + 1) % NUMBER_SEGMENTS;
	DisplayVals* disp = SegmentTarget < 2 ? &SegmentDisplays[0] : &SegmentDisplays[1];
	// Move to the next
	if (disp->doDisplay)
	{
		DisplayOnSegment(&Segments[SegmentTarget], SegmentTarget % 2 == 0 ? disp->left : disp->right, false);	
	}
}

void InitDisplayTimer()
{
	//TODO setup a persistence timer here, to just constantly oscilate through.
}

void SetDisplayNumeric(Displays display, uint8_t val, bool doDisplay)
{
	if (val > 99)
	{
		val = 99;
	}
	SegmentDisplays[display].doDisplay = doDisplay;
	SegmentDisplays[display].left = (DisplayStates)val / 10 ;
	SegmentDisplays[display].right = (DisplayStates)val % 10 ;
}