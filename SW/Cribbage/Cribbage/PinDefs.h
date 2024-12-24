#pragma once
#include <stdint.h>
#include <stm32g4xx.h>
typedef struct
{
	uint32_t pinNumber;
	GPIO_TypeDef* pinPort;
}PinDef;

static const PinDef I2C_SCL = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOB };
static const PinDef I2C_SDA = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOB };
