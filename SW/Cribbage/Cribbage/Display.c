#include "Display.h"
#include "SevenSegment.h"

#define NUMBER_SEGMENTS 4
#define NUMBER_DISPLAYS NUMBER_SEGMENTS/2

static TIM_HandleTypeDef htim1 = { 0 };
static TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
static TIM_MasterConfigTypeDef sMasterConfig = { 0 };
static bool displayLeft = false;

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

static inline void InitPins()
{
	GPIO_InitTypeDef init = { 0 };
	init.Alternate = 0;
	init.Mode = GPIO_MODE_OUTPUT_PP;
	init.Speed = GPIO_SPEED_FREQ_LOW;
	init.Pull = GPIO_NOPULL;
	
	init.Pin = A_EN.pinNumber;
	HAL_GPIO_Init(A_EN.pinPort, &init);
	init.Pin = B_EN.pinNumber;
	HAL_GPIO_Init(B_EN.pinPort, &init);
	init.Pin = C_EN.pinNumber;
	HAL_GPIO_Init(C_EN.pinPort, &init);
	init.Pin = D_EN.pinNumber;
	HAL_GPIO_Init(D_EN.pinPort, &init);
	init.Pin = E_EN.pinNumber;
	HAL_GPIO_Init(E_EN.pinPort, &init);
	init.Pin = F_EN.pinNumber;
	HAL_GPIO_Init(F_EN.pinPort, &init);
	init.Pin = G_EN.pinNumber;
	HAL_GPIO_Init(G_EN.pinPort, &init);
	
	init.Pin = D1_R_EN.pinNumber;
	HAL_GPIO_Init(D1_R_EN.pinPort, &init);
	init.Pin = D2_R_EN.pinNumber;
	HAL_GPIO_Init(D2_R_EN.pinPort, &init);
	init.Pin = D1_G_EN.pinNumber;
	HAL_GPIO_Init(D1_G_EN.pinPort, &init);
	init.Pin = D2_G_EN.pinNumber;
	HAL_GPIO_Init(D2_G_EN.pinPort, &init);
}

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

void TIM3_IRQHandler()
{
	DisplayTimerInterrupt();
}

void InitDisplayTimer()
{
	__TIM3_CLK_ENABLE();
	HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	InitPins();
	
	htim1.Instance = TIM3;
	htim1.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	htim1.Init.Prescaler = 4; // TODO
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP; // TODO
	htim1.Init.Period = 4; // TODO
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4; // TODO
	htim1.Init.RepetitionCounter = 1; // TODO
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // TODO
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		// TODO err
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		// TODO err
	}
	
	// TODO just for testing.
	SetDisplayNumeric(DisplayRed, 3, true);
	SetDisplayNumeric(DisplayGreen, 6, true);
	
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
	
}