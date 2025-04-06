#include "PegHandler.h"
#include "ExpanderController.h"
#include "stm32g4xx_hal.h"
#include "PinDefs.h"
#include "Display.h"

#define BUFFER_LEN NUMBER_EXPANDERS * 2 
#define TOTAL_LENGTH 60

typedef enum 
{
	WaitingForInitCondition = 0,
	Initialized             = 1,
	Running                 = 2,
	Looped					= 3,
	EndGame                 = 4,
}PegStateMachine;

typedef struct
{
	uint64_t data;
	bool Start0;
	bool Start1;
	bool End;
}PegData;

static const uint8_t maxCribbageHand = 29;
static const uint8_t InitMask = 0x03;
static const uint8_t InitOffset = 62;

static bool redLooped = false;
static PegData RedBuffer = { 0 };
static PegData LastRedBuffer = { 0 };
static uint8_t redPoints = 0;
static bool greenLooped = false;
static PegData GreenBuffer = { 0 };
static PegData LastGreenBuffer = { 0 };
static uint8_t greenPoints = 0;
static PegStateMachine currentState = WaitingForInitCondition;

// Count Trailing Zeros
static inline uint8_t CTZ(uint64_t val)
{
	uint8_t retVal = 63;
	while ((val & (1 >> retVal)) == 0)
	{
		retVal--;
	}
	
	return retVal;
}

// Count leading zeros.
static inline uint8_t CLZ(uint64_t val)
{
	uint8_t retVal = 0;
	while ((val & (1 >> retVal)) == 0)
	{
		retVal++;
	}
	
	return retVal;
}

typedef struct 
{
	uint8_t first;
	uint8_t second;
}PegPositions;


static inline PegPositions getPegPositions(uint64_t buffer)
{
	PegPositions pos = { 0, 0 };
	pos.first = CLZ(buffer);
	pos.second = CTZ(buffer);
	return pos;
}

static void InterpretPegs(PegData lastBuffer, PegData newBuffer, uint8_t* delta, uint8_t lastTotalPoint)
{
	// TODO lots of testing here.
	// TODO write a stub of code to test this in place of unit testing.
	PegPositions lastLocations = getPegPositions(lastBuffer.data);
	PegPositions newLocations = getPegPositions(newBuffer.data); 
	// Thought process. 
	// First check if last had both in init
	if (lastBuffer.Start0 == true && lastBuffer.Start1 == true) 
	{
		// If some amount of points were scored, the delta is the exact position -1
		if (newBuffer.data != 0)
		{
			*delta = newLocations.first - 1;
		}
	}
	// TODO wrap around needs definition. Can max hadn in cribbage be used here to estimate if someone did?
}

void InitPegs()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = Start_00.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(Start_00.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = Start_01.pinNumber;
	HAL_GPIO_Init(Start_01.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = Start_10.pinNumber;
	HAL_GPIO_Init(Start_10.pinPort, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = Start_11.pinNumber;
	HAL_GPIO_Init(Start_11.pinPort, &GPIO_InitStruct);
}

void UpdateBankInfo()
{
	uint64_t leftBank = 0;
	uint64_t rightBank = 0;
	for (uint8_t i = 0; i < NUMBER_EXPANDERS; i++)
	{
		uint16_t bankInfo = GetBankReadings(i);
		uint8_t left = bankInfo & 0x00ff;
		uint8_t right = bankInfo >> 8;
		leftBank |= left << (i * 5);
		rightBank |= right << (i * 5);
	}
	LastGreenBuffer = GreenBuffer;
	LastRedBuffer = RedBuffer;
	GreenBuffer.data = leftBank;
	GreenBuffer.Start0 = HAL_GPIO_ReadPin(Start_00.pinPort, Start_00.pinNumber) == GPIO_PIN_SET;
	GreenBuffer.Start1 = HAL_GPIO_ReadPin(Start_01.pinPort, Start_01.pinNumber) == GPIO_PIN_SET;
	GreenBuffer.End = HAL_GPIO_ReadPin(FinalPin.pinPort, FinalPin.pinNumber) == GPIO_PIN_SET;
	
	RedBuffer.data = rightBank;
	RedBuffer.Start0 = HAL_GPIO_ReadPin(Start_10.pinPort, Start_10.pinNumber) == GPIO_PIN_SET;
	RedBuffer.Start1 = HAL_GPIO_ReadPin(Start_11.pinPort, Start_11.pinNumber) == GPIO_PIN_SET;
	RedBuffer.End = HAL_GPIO_ReadPin(FinalPin.pinPort, FinalPin.pinNumber) == GPIO_PIN_SET;
}

void HandlePegStateMachine()
{
	switch (currentState)
	{
	case WaitingForInitCondition:
		{
			if (RedBuffer.Start0 == true && RedBuffer.Start1 == true &&
				GreenBuffer.Start0 == true && GreenBuffer.Start1 == true)
			{
				currentState = Initialized;
			}
			// TODO display differently.
		}
		break;
	case Initialized:
		{
			
		}
		break;
	case Running:
		{
			
		}
		break;
	case Looped:
		{
			
		}
	case EndGame:
		{
			
		}
		break;
	default:
		{
			
		}
		break;
	}
	RunDisplayStateMachine();
}