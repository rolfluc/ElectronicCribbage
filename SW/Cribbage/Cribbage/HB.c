#include "HB.h"
#include "PinDefs.h"

void InitHB()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = HB.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(HB.pinPort, &GPIO_InitStruct);
}

static GPIO_PinState last = GPIO_PIN_RESET;
void ToggleHB()
{
	if (last == GPIO_PIN_RESET)
	{
		last = GPIO_PIN_SET;
		HAL_GPIO_WritePin(HB.pinPort, HB.pinNumber, last);
	}
	else
	{
		last = GPIO_PIN_RESET;
		HAL_GPIO_WritePin(HB.pinPort, HB.pinNumber, last);
	}
	
}