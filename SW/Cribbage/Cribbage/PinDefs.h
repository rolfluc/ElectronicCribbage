#pragma once
#include <stdint.h>
#include <stm32c0xx.h>
typedef struct
{
	uint32_t pinNumber;
	GPIO_TypeDef* pinPort;
}PinDef;

static const PinDef I2C_SCL = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOB };
static const PinDef I2C_SDA = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOB };

static const PinDef D2_G_EN = { .pinNumber = GPIO_PIN_0, .pinPort = GPIOA };
static const PinDef D1_G_EN = { .pinNumber = GPIO_PIN_1, .pinPort = GPIOA };
static const PinDef D_EN = { .pinNumber = GPIO_PIN_2, .pinPort = GPIOA };
static const PinDef E_EN = { .pinNumber = GPIO_PIN_3, .pinPort = GPIOA };
static const PinDef C_EN = { .pinNumber = GPIO_PIN_4, .pinPort = GPIOA };
static const PinDef G_EN = { .pinNumber = GPIO_PIN_5, .pinPort = GPIOA };
static const PinDef A_EN = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOA };
static const PinDef F_EN = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOA };
static const PinDef B_EN = { .pinNumber = GPIO_PIN_8, .pinPort = GPIOA };
static const PinDef D2_R_EN = { .pinNumber = GPIO_PIN_9, .pinPort = GPIOA };
static const PinDef D1_R_EN = { .pinNumber = GPIO_PIN_10, .pinPort = GPIOA };
