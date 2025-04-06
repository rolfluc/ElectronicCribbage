#include "PegHandler.h"
#include "ExpanderController.h"

#define BUFFER_LEN NUMBER_EXPANDERS * 2 
#define TOTAL_LENGTH 60

typedef enum 
{
	WaitingForInitCondition = 0,
	Initialized             = 1,
	Running                 = 2,
	EndGame                 = 3,
}PegStateMachine;

static const uint8_t InitMask = 0x03;
static const uint8_t InitOffset = 62;

static bool redLooped = false;
static uint64_t RedBuffer = 0;
static uint64_t LastRedBuffer = 0;
static uint8_t redPoints = 0;
static bool greenLooped = false;
static uint64_t GreenBuffer = 0;
static uint64_t LastGreenBuffer = 0;
static uint8_t greenPoints = 0;
static PegStateMachine currentState = WaitingForInitCondition;

static bool didLoop(uint64_t oldBuffer, uint64_t newBuffer)
{
	bool didLoop = false;
	// TODO how to do this.
}

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

static void InterpretPegs(uint64_t lastBuffer, uint64_t newBuffer, uint8_t* delta, uint8_t lastTotalPoint)
{
	// If either are zero, undefined operations. Exit quickly. No Pegs No Display.
	if (lastBuffer == 0 || newBuffer == 0)
	{
		return;
	}
	
	PegPositions lastLocations = getPegPositions(lastBuffer);
	PegPositions newLocations = getPegPositions(newBuffer); 
	// Thought process. 
}

static void GetOnBoardPegInfo()
{
	
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
	GreenBuffer = leftBank;
	RedBuffer = rightBank;
}

void HandlePegStateMachine()
{
	switch (currentState)
	{
	case WaitingForInitCondition:
		{
			bool greenGood = false;
			bool redGood = false;
			// Waiting for init condition, should be the following:
			// Both red and green banks, should have their respective pegs adjacent to each other, 
			// in -2, and -1 positions.
			if (GreenBuffer & ((uint64_t)InitMask << InitOffset))
			{
				greenGood  = true;
				
			}
			if (RedBuffer & ((uint64_t)InitMask << InitOffset))
			{
				redGood  = true;
				
			}
			if (redGood && greenGood)
			{
				currentState = Initialized;
			}
		}
		break;
	case Initialized:
		{
			// TODO what does this mean.
		}
		break;
	case Running:
		{
			
		}
		break;
	case EndGame:
		{
			
		}
		break;
	default:
		{
			
		}
		break;
	}		 
}