#include "ADC.h"
#include "PinDefs.h"
#include "stm32g4xx.h"

ADC_HandleTypeDef adcHandle;

void InitADC()
{
	__HAL_RCC_ADC12_CLK_ENABLE();
	GPIO_InitTypeDef init;
	
	adcHandle.Instance = ADC1;
	adcHandle.Init.ClockPrescaler =  ADC_CLOCK_ASYNC_DIV1;
	adcHandle.Init.Resolution = ADC_RESOLUTION_12B;
	// adcHandle.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
	adcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adcHandle.Init.LowPowerAutoWait = DISABLE;
	adcHandle.Init.ContinuousConvMode = DISABLE;
	adcHandle.Init.DiscontinuousConvMode = DISABLE;
	adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adcHandle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	if (HAL_ADC_Init(&adcHandle) != HAL_OK)
	{
		//Error_Handler();
	}
	
	init.Mode = GPIO_MODE_ANALOG;
	init.Pull = GPIO_NOPULL;
	init.Pin = ADC.pinNumber;
	HAL_GPIO_Init(ADC.pinPort, &init);
}

uint16_t GetADCCount()
{
	uint16_t retVal = 0;
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_0;
	// sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	// sConfig.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
	if (HAL_ADC_ConfigChannel(&adcHandle, &sConfig) != HAL_OK)
	{
		//Error_Handler();
	}
	HAL_ADC_Start(&adcHandle);
	HAL_ADC_PollForConversion(&adcHandle, 10000);
	retVal = (uint16_t)HAL_ADC_GetValue(&adcHandle);
	HAL_ADC_Stop(&adcHandle);
	return retVal;
}
