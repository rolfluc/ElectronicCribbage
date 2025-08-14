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
	Initialized             = 1,
	Running                 = 2,
	EndGame                 = 3,
}PegStateMachine;

typedef struct
{
	uint64_t data;
	bool Start0;
	bool Start1;
	bool End;
}PegData;

static const uint8_t maxCribbageHand = 29;

static bool redLooped = false;
static PegData RedBuffer = { 0 };
static PegData LastRedBuffer = { 0 };
static uint8_t redPoints = 0;
static bool greenLooped = false;
static PegData GreenBuffer = { 0 };
static PegData LastGreenBuffer = { 0 };
static uint8_t greenPoints = 0;
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

static const char* InitText = "ceased";
static const char* WinText = "F1n1sh";
static Color c;

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
			uint8_t greenDelta = 0;
			uint8_t redDelta = 0;
			// Two conditions here. Comparing two pegs in field, or peg in field against start peg.
			if (RedBuffer.Start0 == false && RedBuffer.Start1 == false) {
				uint8_t trailing = CTZ(RedBuffer.data);
				uint8_t leading = CLZ(RedBuffer.data);
				// If these two equal length - 1, means only 1 peg inserted. Set to invalid value
				if (trailing + leading == TOTAL_LENGTH - 1) {
					redDelta = 0xff;
				}
				else {
					// Example measurement. If trailing zeros is 10, we have a peg at 11.
					// If leading zeros is 30, we have a peg at 29. In this scenario, we want to subtract
					// we want to subtract trailing zeros from leading zeros to get the valid result.
					redDelta = leading - trailing;
					if (redDelta > maxCribbageHand) {

					}
				}
			}
			else if ((RedBuffer.Start0 == true || RedBuffer.Start1 == true) && RedBuffer.data > 0) {
				redDelta = CTZ(RedBuffer.data);
				c = ColorRed;
				FillNumberBuffer(numberBuffer, redDelta);
				SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
			}
			else if ((GreenBuffer.Start0 == true || GreenBuffer.Start1 == true) && GreenBuffer.data > 0) {
				greenDelta = CTZ(GreenBuffer.data);
				c = ColorGreen;
				FillNumberBuffer(numberBuffer, greenDelta);
				SetSystemText(c, (char*)numberBuffer, strlen(numberBuffer));
			}
			else if (GreenBuffer.End == true) {
				currentState = EndGame;
				// Migrated, rerun. Warning recursive.
				HandlePegStateMachine();
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
			if (GreenBuffer.data > 0 || RedBuffer.data > 0) {
				currentState = Running;
				// State transitioned, rerun. 
				HandlePegStateMachine(); // Warning Recursion.
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