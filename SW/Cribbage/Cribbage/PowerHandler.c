#include "PowerHandler.h"
#include "stm32g4xx_hal.h"
#include "PinDefs.h"
#include <stdbool.h>

static inline void TurnOffPower()
{
	// TODO need to turn on 7 segment 88 White, to consume the most current to drain capacitors faster.
	HAL_GPIO_WritePin(PowerOff.pinPort, PowerOff.pinNumber, GPIO_PIN_RESET);
}
static inline void KeepPowerOn()
{
	HAL_GPIO_WritePin(PowerOff.pinPort, PowerOff.pinNumber, GPIO_PIN_SET);
}

static const uint32_t TOO_EARLY_TOO_SHUTDOWN = 10000;
static const uint32_t SHUTOFF_HOLD_TIME = 2000;
static uint32_t timeStart_ms = 0;
static GPIO_PinState lastState = GPIO_PIN_SET;
void HandleButtonPress()
{
	uint32_t currentTime = HAL_GetTick();
	// When we go from pressed, to depressed, and its been held for 2s, then turn itself off.
	GPIO_PinState currentState = HAL_GPIO_ReadPin(ButtonSense.pinPort, ButtonSense.pinNumber);
	if (lastState == GPIO_PIN_SET && currentState == GPIO_PIN_RESET && currentTime > timeStart_ms + SHUTOFF_HOLD_TIME && timeStart_ms > TOO_EARLY_TOO_SHUTDOWN)
	{
		TurnOffPower();
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
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = PowerOff.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(PowerOff.pinPort, &GPIO_InitStruct);
	
	// Need to immediately fire this on while the button is stabalizing.
	KeepPowerOn();
}

void InitPowerSense()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = ButtonSense.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ButtonSense.pinPort, &GPIO_InitStruct);
	
	//HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void DetermineSufficientVoltage(uint16_t counts)
{
	static const uint16_t CUTOFF_COUNT = 0x0744; // TODO
	// Voltage divier 1/2.
	// Vsys = 3.3V
	// Voltage max = 4.2V
	// Voltage min = 3.0V
	// 0x0fff = 4095
	// 4.2 / 2 = 2.1
	// 2.1 / 3.3 = .63
	// 3.0 / 2 = 1.5
	// 1.5 / 3.3 = .45
	// .45 * 4095 = 1860
	if (counts < CUTOFF_COUNT)
	{
		// TODO probably show low voltage then cutout.
		TurnOffPower();
	}
}