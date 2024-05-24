#include "Display.h"
#include "SevenSegment.h"

#define NUMBER_SEGMENTS 4
#define NUMBER_DISPLAYS NUMBER_SEGMENTS/2

typedef struct
{
	DisplayStates state;
	Segment* segment;
}SegmentConfig;

typedef struct 
{
	bool doDisplay;
	SegmentConfig* left;
	SegmentConfig* right;
}DisplayVals;

static Segment Segments[NUMBER_SEGMENTS] = { 
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D1_R_EN, true},
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D2_R_EN, true},
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D1_G_EN, true},
	{&A_EN, &B_EN, &C_EN, &D_EN, &E_EN, &F_EN, &G_EN, NULL, &D2_G_EN, true},
};
static SegmentConfig RedLeft = { 
	Display_Nothing,
	&Segments[0]
};
static SegmentConfig RedRight = { 
	Display_Nothing,
	&Segments[1]
};

static SegmentConfig GreenLeft = { 
	Display_Nothing,
	&Segments[2]
};

static SegmentConfig GreenRight = { 
	Display_Nothing,
	&Segments[3]
};

static DisplayVals RedDisplay = { 
	false, &RedLeft, &RedRight	
};

static DisplayVals GreenDisplay = { 
	false, &GreenLeft, &GreenRight	
};

static TIM_HandleTypeDef htim1;
static bool displayLeft = false;

static inline void DisplayLeft()
{
	TurnOffSegment(RedDisplay.right->segment);	
	TurnOffSegment(GreenDisplay.right->segment);
	if (RedDisplay.doDisplay)
	{
		DisplayOnSegment(RedDisplay.left->segment, RedDisplay.left->state, false);
	}
	if (GreenDisplay.doDisplay)
	{
		DisplayOnSegment(GreenDisplay.left->segment, GreenDisplay.left->state, false);
	}
}

static inline void DisplayRight()
{
	TurnOffSegment(RedDisplay.left->segment);
	TurnOffSegment(GreenDisplay.left->segment);
	if (RedDisplay.doDisplay)
	{
		DisplayOnSegment(RedDisplay.right->segment, RedDisplay.right->state, false);
	}
	if (GreenDisplay.doDisplay)
	{
		DisplayOnSegment(GreenDisplay.right->segment, GreenDisplay.right->state, false);
	}
}

static inline void DisplayTimerInterrupt()
{
	displayLeft ? DisplayLeft() : DisplayRight();
	displayLeft = !displayLeft;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// TODO clear timer irq?
	DisplayTimerInterrupt();
}

void InitDisplayTimer()
{
	HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	htim1.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	htim1.Instance = TIM3;
	htim1.Init.Prescaler = 4; // TODO
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP; // TODO
	htim1.Init.Period = 4; // TODO
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; // TODO
	htim1.Init.RepetitionCounter = 1; // TODO
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // TODO
	HAL_TIM_Base_Start_IT(&htim1);
}

void SetDisplayNumeric(Displays display, uint8_t val, bool doDisplay)
{
	if (val > 99)
	{
		val = 99;
	}
	DisplayVals* target;
	if (display == DisplayRed)
	{
		target = &RedDisplay;
	}
	else
	{
		target = &GreenDisplay;
	}
	target->doDisplay = doDisplay;
	target->left->state = (DisplayStates)(val / 10);
	target->right->state = (DisplayStates)(val % 10);
}

void SetCharacterDisplay(Displays display, char left, char right, bool doDisplay)
{
	if 
}