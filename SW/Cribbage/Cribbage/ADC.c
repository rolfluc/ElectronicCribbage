#include "ADC.h"
#include "PinDefs.h"
#include "stm32g4xx.h"

ADC_HandleTypeDef adcHandle;

void InitADC()
{
	__HAL_RCC_ADC12_CLK_ENABLE();
	GPIO_InitTypeDef init;
	ADC_MultiModeTypeDef multimode = { 0 };
	ADC_ChannelConfTypeDef sConfig = { 0 };
	
	adcHandle.Instance = ADC1;
	adcHandle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adcHandle.Init.Resolution = ADC_RESOLUTION_12B;
	adcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adcHandle.Init.GainCompensation = 0;
	adcHandle.Init.ScanConvMode = ADC_SCAN_DISABLE;
	adcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adcHandle.Init.LowPowerAutoWait = DISABLE;
	adcHandle.Init.ContinuousConvMode = DISABLE;
	adcHandle.Init.NbrOfConversion = 1;
	adcHandle.Init.DiscontinuousConvMode = DISABLE;
	adcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adcHandle.Init.DMAContinuousRequests = DISABLE;
	adcHandle.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	adcHandle.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&adcHandle) != HAL_OK)
	{
	}

	/** Configure the ADC multi-mode
	*/
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&adcHandle, &multimode) != HAL_OK)
	{
	}

	/** Configure Regular Channel
	*/
	sConfig.Channel = ADC_CHANNEL_15;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&adcHandle, &sConfig) != HAL_OK)
	{
	}
	
	init.Mode = GPIO_MODE_ANALOG;
	init.Pull = GPIO_NOPULL;
	init.Pin = ADC.pinNumber;
	HAL_GPIO_Init(ADC.pinPort, &init);
}

uint16_t GetADCCount()
{
	uint16_t retVal = 0;
	HAL_ADC_Start(&adcHandle);
	HAL_ADC_PollForConversion(&adcHandle, 10000);
	retVal = (uint16_t)HAL_ADC_GetValue(&adcHandle);
	HAL_ADC_Stop(&adcHandle);
	return retVal;
}
