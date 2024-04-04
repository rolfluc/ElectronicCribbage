#include "PegHandler.h"
#include "ExpanderController.h"

#define BUFFER_LEN NUMBER_EXPANDERS * 2 

uint64_t RedBuffer = 0;
uint64_t GreenBuffer = 0;

static bool didLoop(uint64_t oldBuffer, uint64_t newBuffer)
{
	bool didLoop = false;
	// TODO how to do this.
}

void ReadAllPegs()
{
	uint8_t currentByte = 0;
	uint16_t retVal = 0;
	uint8_t loopTarget = 0;
	uint64_t tmpRedBuffer = 0;
	uint64_t tmpGreenBuffer = 0;
	for (uint8_t i = 0; i < NUMBER_EXPANDERS; i++)
	{
		TurnB0(i, true);
		StartBankReading(i);
		retVal = GetBankReadings(i);
		currentByte = retVal & 0x001f;
		tmpRedBuffer |= currentByte << (loopTarget * 5);
		currentByte = retVal >> 8;
		tmpGreenBuffer |= currentByte << (loopTarget++ * 5);
		TurnB0(i, false);
		
		TurnB1(i, true);
		StartBankReading(i);
		retVal = GetBankReadings(i);
		currentByte = retVal & 0x001f;
		tmpRedBuffer |= currentByte << (loopTarget * 5);
		currentByte = retVal >> 8;
		tmpGreenBuffer |= currentByte << (loopTarget++ * 5);
		TurnB1(i, false);
	}
}

inline uint8_t GetDelta(uint64_t buffer, bool looped)
{
	bool foundStart = false;
	bool foundBoth = false;
	uint8_t startBit = 0;
	uint8_t endBit = 0;
	uint8_t delta = 0;
	for (uint8_t i = 0; i < 64; i++)
	{
		if (buffer & (1 << i))
		{
			if (!foundStart)
			{
				startBit = i;	
				foundStart = true;
			}
			else
			{
				endBit = i;
				foundBoth = true;
				break;
			}
		}
	}
	if (looped)
	{
		
	}
	return delta;
}

uint8_t GetRedDelta(bool looped)
{
	return GetDelta(RedBuffer, looped);
}

uint8_t GetGreenDelta(bool looped)
{
	return GetDelta(GreenBuffer, looped);
}