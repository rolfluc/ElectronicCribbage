#include <stm32c0xx_hal.h>
#include <stm32_hal_legacy.h>
#include "I2C.h"
#include "ExpanderController.h"
#include "Display.h"

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

int main(void)
{
	HAL_Init();

	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	InitExpanders();
	InitDisplayTimer();

	for (;;)
	{
		
		
		HAL_Delay(125);
	}
}
