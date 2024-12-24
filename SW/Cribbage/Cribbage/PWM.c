#include "PWM.h"
#include "stm32g4xx.h"
#include "LEDTiming.h"

TIM_HandleTypeDef htim;
DMA_HandleTypeDef hdma_tim1_ch1;
uint8_t* bufferTarget = 0;
uint16_t currentTarget = 0;
uint16_t maxLength = 0;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
}

void DMA1_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_tim1_ch1);
}

static void MX_DMA_Init(void)
{
	/* DMA controller clock enable */
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

static void InitGPIOs()
{
	__GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF6_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_Init(void)
{
	__HAL_RCC_TIM1_CLK_ENABLE();
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };
	
	InitGPIOs();
	hdma_tim1_ch1.Instance = DMA1_Channel1;
	hdma_tim1_ch1.Init.Request = DMA_REQUEST_TIM1_CH1;
	hdma_tim1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_tim1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_tim1_ch1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_ch1.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_tim1_ch1.Init.Mode = DMA_NORMAL;
	hdma_tim1_ch1.Init.Priority = DMA_PRIORITY_LOW;
	if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	__HAL_LINKDMA(&htim, hdma[TIM_DMA_ID_CC1], hdma_tim1_ch1);
	
	htim.Instance = TIM1;
	htim.Init.Prescaler = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = PERIOD_COUNTS;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.RepetitionCounter = 0;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.BreakFilter = 0;
	sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
	sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
	sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
	sBreakDeadTimeConfig.Break2Filter = 0;
	sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim, &sBreakDeadTimeConfig) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
}


void InitTimer()
{
	MX_DMA_Init();
	MX_Init();
}

void SendPWMDMA(uint8_t* buffer, uint16_t count)
{
	HAL_TIM_PWM_Start_DMA(&htim, TIM_CHANNEL_1, (uint32_t*)buffer, count);
}