#include "PegHandler.h"
#include "ExpanderController.h"
#include "stm32g4xx_hal.h"
#include "PinDefs.h"
#include "Display.h"
#include <string.h>

#define BUFFER_LEN NUMBER_EXPANDERS * 2 
#define TOTAL_LENGTH 60

typedef enum 
{
	WaitingForInitCondition = 0,
	Initialized,
	GreenStarted,
	RedStarted,
	BothStarted,
	Running,
	EndGame,
}PegStateMachine;

typedef struct
{
	uint64_t data;
	bool Start0;
	bool Start1;
	bool End;
}PegData;

static const uint32_t updateRate_ms = 3000;
static const uint8_t maxCribbageHand = 29;
static const char* InitText = "ceased";
static const char* WinText = "F1n1sh";
static Color c;
static uint32_t lastTransitionTime_ms = 0; 
static bool lastDisplayedRed = false;

static PegData RedBuffer = { 0 };
static PegData LastRedBuffer = { 0 };
static PegData GreenBuffer = { 0 };
static PegData LastGreenBuffer = { 0 };
static PegStateMachine currentState = WaitingForInitCondition;


static inline bool DidLoop(uint8_t firstPos, uint8_t secondPos)
{
	return (secondPos - firstPos) > maxCribbageHand;
}

// Count Trailing Zeros
static inline uint8_t CTZ(uint64_t val)
{
	if (val == 0) {
		return 0;
	}
	uint8_t retVal = TOTAL_LENGTH;
	while ((val & (1 << retVal)) == 0)
	{
		retVal--;
	}
	
	return retVal + 1;
}

// Count leading zeros.
static inline uint8_t CLZ(uint64_t val)
{
	if (val == 0) {
		return 0;
	}
	uint8_t retVal = 0;
	while ((val & (1 >> retVal)) == 0)
	{
		retVal++;
	}
	
	return retVal;
}

// Thought process:
// 000000000000000000000000000000000000000000000000000000000000
// is clearly, nothing detected.
// 000000000000000000000000000000000000000000000000000000000001
// Has one peg, in the first position.
// 000000000000000000000000000000000000000000000000000000000010
// has one peg, in 2nd position, leading to 2 points.
// 000000000000000000000000000000000000000000000000000000000011
// Has 2 pega, one in the 2nd, and 1 in the first, leading to 1 pt.
// This math can be performed by CTZ() - which would return 1, 
// and CLZ would return 58. 60 - 58 - 1 = 1
// 000000000000000000000000000000000000000000000000000000010010
// has a peg in points position 2, and a peg in points position 5. 
// Actual points is 3. CTZ() will return 2, CLZ() will return 55
// 60 - 55 - 2 = 3
// 010000000000000000000000000000000000000000000000000000000010
// Has pega in position 2 and position 58. This is clearly a loop.
// CTZ() will return 2. CLZ() will return 1. 
// 60 - 1 - 2 = 57.  Technically, the last one is "in front" of the first.
// So if the delta is > Max Hand Size, you take the above and take 60-res. 

static inline uint8_t getDelta(uint64_t bufferData) {
	uint8_t ctz = CTZ(bufferData);
	uint8_t clz = CLZ(bufferData);
	// Check as to whether if CTZ + CLZ = total length.
	// In this case, return 0.
	if (ctz + clz == TOTAL_LENGTH) {
		return 0;
	}
	uint8_t delta = TOTAL_LENGTH - clz - ctz;
	if (delta > maxCribbageHand) {
		delta = TOTAL_LENGTH - delta;
	}
	return delta;
}

void InitPegs()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = Start_00.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
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
		// then shifts into the right position.
		uint32_t bankInfo = GetBankReadings(i);
		uint64_t left = bankInfo & 0x000003ff;
		uint64_t right = (bankInfo >> 16) & 0x000003ff;
		leftBank |= left << (i * 10);
		rightBank |= right << (i * 10);
	}
	LastGreenBuffer = GreenBuffer;
	LastRedBuffer = RedBuffer;
	GreenBuffer.data = leftBank;
	GreenBuffer.Start0 = HAL_GPIO_ReadPin(Start_00.pinPort, Start_00.pinNumber) == GPIO_PIN_RESET;
	GreenBuffer.Start1 = HAL_GPIO_ReadPin(Start_01.pinPort, Start_01.pinNumber) == GPIO_PIN_RESET;
	// TODO check this logic.  Rainbow "Game Over" scenario. Green vs red cannot be explicitly determined 
	// for win scenario.
	GreenBuffer.End = HAL_GPIO_ReadPin(FinalPin.pinPort, FinalPin.pinNumber) == GPIO_PIN_SET;
	
	RedBuffer.data = rightBank;
	RedBuffer.Start0 = HAL_GPIO_ReadPin(Start_10.pinPort, Start_10.pinNumber) == GPIO_PIN_RESET;
	RedBuffer.Start1 = HAL_GPIO_ReadPin(Start_11.pinPort, Start_11.pinNumber) == GPIO_PIN_RESET;
	// RedBuffer.End = HAL_GPIO_ReadPin(FinalPin.pinPort, FinalPin.pinNumber) == GPIO_PIN_SET;
}

static void FillNumberBuffer(char* buffer, uint8_t num) {
	if (num < 10) {
		buffer[0] = '0';
		buffer[1] = num + '0';
	} else {
		buffer[0] = (num / 10) + '0';
		buffer[1] = (num % 10) + '0';
	}
}

void HandlePegStateMachine()
{
	char numberBuffer[2] = "";
	switch (currentState)
	{
		case Running:
		{
			uint32_t currentTime = HAL_GetTick();
			if (!GreenBuffer.End) {
				// Check if either buffer has changed, and that there are two pegs in field.
				// If either player does not have two pegs, do not display them.
				// If one changed, and has both in field, display it immediately.
				// If neither changed, alternate between showing their Current point totals,
				// and their current delta.
				uint8_t redDelta = getDelta(RedBuffer.data);
				uint8_t greenDelta = getDelta(GreenBuffer.data);
				if (RedBuffer.data != LastRedBuffer.data && redDelta > 0) {
					c = ColorRed;	
					FillNumberBuffer(numberBuffer, greenDelta);
					SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
					lastDisplayedRed = true;
					lastTransitionTime_ms = currentTime;
				} else if (GreenBuffer.data != LastGreenBuffer.data && greenDelta > 0) {
					c = ColorGreen;	
					FillNumberBuffer(numberBuffer, greenDelta);
					SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
					lastDisplayedRed = false;
					lastTransitionTime_ms = currentTime;
				} else {
					// Alternate the display state machine.
					if (currentTime > lastTransitionTime_ms + updateRate_ms) {
						uint8_t displayBufferDat = 0;
						lastTransitionTime_ms = currentState;
						if (lastDisplayedRed) {
							// Only show if the delta is non zero.
							if (greenDelta != 0 ) {
								c = ColorGreen;	
								displayBufferDat = greenDelta;
							}
						} else {
							// Only show if the delta is non zero.
							if (redDelta == 0) {
								c = ColorRed;
								displayBufferDat = redDelta;
							}
						}
						FillNumberBuffer(numberBuffer, displayBufferDat);
						SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
						lastDisplayedRed = !lastDisplayedRed;
				}
			} else {
				// End game found. Transition and move on.
				currentState = EndGame;
				HandlePegStateMachine(); // Warning recursion.
			}
		}
		break;
		// Waiting for init, is waiting for the pegs to be in the starting holes.
		case WaitingForInitCondition:
		{
			c = ColorBlue;
			SetSystemText(c, (char*)InitText, strlen(InitText));
			if (RedBuffer.Start0 == true && RedBuffer.Start1 == true &&
				GreenBuffer.Start0 == true && GreenBuffer.Start1 == true)
			{
				currentState = Initialized;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
			}
			break;
		}
		// Initialized, is when the pegs are in the holes, but none have been detected into any of the expander boards.
		case Initialized:
		{
			if (RedBuffer.data > 0 && GreenBuffer.data > 0) {
				currentState = BothStarted;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
			} else if (GreenBuffer.data > 0 && RedBuffer.data == 0) {
				currentState = GreenStarted;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
			} else if (RedBuffer.data > 0 && GreenBuffer.data == 0) {
				currentState = RedStarted;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
			}
			break;
		}
		case GreenStarted:
		{
			uint8_t greenDelta = 0;
			c = ColorGreen;
			FillNumberBuffer(numberBuffer, greenDelta);
			SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));

			if (RedBuffer.data > 0) {
				currentState = BothStarted;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
			}
			break;
		}
		case RedStarted:
		{
			uint8_t redDelta = 0;
			c = ColorRed;
			FillNumberBuffer(numberBuffer, redDelta);
			SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));

			if (GreenBuffer.data > 0) {
				currentState = BothStarted;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
			}
			break;
		}
		case BothStarted:
		{
			uint32_t currentTime = HAL_GetTick();
			// If all the pegs have cleared from their start positions...
			if (GreenBuffer.Start0 == false && GreenBuffer.Start1 == false && 
				RedBuffer.Start0 == false && RedBuffer.Start1 == false) {
				currentState = Running;
				// State transition, rerun.
				HandlePegStateMachine();
			// If The pegs have not all cleared, rotate between showing their positions.
			} else {
				// First, check if either have changed. This will show the change faster
				// If neither Red nor Green have changed, then revert to the following.
				if (RedBuffer.data != LastRedBuffer.data) {
					c = ColorRed;
					uint8_t redDelta = getDelta(RedBuffer.data);
					if (redDelta > 0) {
						FillNumberBuffer(numberBuffer, redDelta);
						SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
						lastDisplayedRed = true;	
					}
				} else if (GreenBuffer.data != LastGreenBuffer.data) {
					c = ColorGreen;
					uint8_t greenDelta = getDelta(GreenBuffer.data);
					if (greenDelta > 0) {
						FillNumberBuffer(numberBuffer, greenDelta);
						SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
						lastDisplayedRed = false;	
					}
				} else {
					// If we have not started on both, rotate between the two's positions.
					if (currentTime > lastTransitionTime_ms + updateRate_ms) {
						uint8_t displayBufferDat = 0;
						lastTransitionTime_ms = currentTime;
						if (lastDisplayedRed) {
							c = ColorGreen;	
							displayBufferDat = getDelta(GreenBuffer.data);
							if (displayBufferDat == 0) {
								displayBufferDat = CTZ(GreenBuffer.data);
							}
						} else {
							c = ColorRed;
							displayBufferDat = getDelta(RedBuffer.data);
							if (displayBufferDat == 0) {
								displayBufferDat = CTZ(RedBuffer.data);
							}
						}
						FillNumberBuffer(numberBuffer, displayBufferDat);
						SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
						lastDisplayedRed = !lastDisplayedRed;
					}	
				}
			}
			break;
		} 
		// End-game, is when a peg is in the final hole. 
		case EndGame:
		{
			c = ColorWhite;
			SetSystemText(c, (char*)WinText, strlen(WinText));
			break;
		}
		default:
		{
			// Undefined behavior.
			break;	
		}
	}
	RunDisplayStateMachine();
}