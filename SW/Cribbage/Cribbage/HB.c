#include "HB.h"
#include "PinDefs.h"

void InitHB()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = HB.pinNumber;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(HB.pinPort, &GPIO_InitStruct);
}

void ToggleHB()
{
	HAL_GPIO_TogglePin(HB.pinPort, HB.pinNumber);
}