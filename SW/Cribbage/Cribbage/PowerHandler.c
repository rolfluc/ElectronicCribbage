#include "PowerHandler.h"
#include "stm32g4xx_hal.h"
#include "PinDefs.h"
#include <stdbool.h>
#include "Display.h"

static const uint32_t SHOW_BATTERY_TIMEOUT = 250;
static const uint32_t RESET_GAME_TIME_MS = 5000;
static uint32_t timeStart_ms = 0;
static GPIO_PinState lastState = GPIO_PIN_SET;
void HandleButtonPress()
{
	uint32_t currentTime = HAL_GetTick();
	GPIO_PinState currentState = HAL_GPIO_ReadPin(ButtonSense.pinPort, ButtonSense.pinNumber);
	if (lastState == GPIO_PIN_SET && currentState == GPIO_PIN_RESET)
	{
		// Show battery timeout
		if (currentTime > timeStart_ms + SHOW_BATTERY_TIMEOUT && currentTime < timeStart_ms + SHOW_BATTERY_TIMEOUT) {
			__ASM("BKPT 255");
		// Reset game timeout
		} else if (currentTime > timeStart_ms + RESET_GAME_TIME_MS) {
			__ASM("BKPT 255");
		}
	}
	if (lastState != currentState)
	{
		timeStart_ms = currentTime;
	}
	lastState = currentState;
}

void EXTI4_IRQHandler(void)
{
	HandleButtonPress();
}


void InitializePowerPin()
{
	// No longer used. Set as an input to let bygones be bygones.
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = PowerOff.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(PowerOff.pinPort, &GPIO_InitStruct);
}

void InitPowerSense()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = ButtonSense.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ButtonSense.pinPort, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}
