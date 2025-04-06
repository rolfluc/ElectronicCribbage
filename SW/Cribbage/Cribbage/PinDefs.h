#pragma once
#include <stdint.h>
#include <stm32g4xx.h>
typedef struct
{
	uint32_t pinNumber;
	GPIO_TypeDef* pinPort;
}PinDef;



static const PinDef I2C_SCL = { .pinNumber = GPIO_PIN_15, .pinPort = GPIOA };
static const PinDef I2C_SDA = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOB };

static const PinDef Start_00 = { .pinNumber = GPIO_PIN_0, .pinPort = GPIOA };
static const PinDef Start_01 = { .pinNumber = GPIO_PIN_1, .pinPort = GPIOA };
static const PinDef Start_10 = { .pinNumber = GPIO_PIN_2, .pinPort = GPIOA };
static const PinDef Start_11 = { .pinNumber = GPIO_PIN_3, .pinPort = GPIOA };
static const PinDef FinalPin = { .pinNumber = GPIO_PIN_6, .pinPort = GPIOA };

static const PinDef ButtonSense = { .pinNumber = GPIO_PIN_4, .pinPort = GPIOA };

static const PinDef Display_COUT = { .pinNumber = GPIO_PIN_5, .pinPort = GPIOA };
static const PinDef Display_DOUT = { .pinNumber = GPIO_PIN_7, .pinPort = GPIOA };

static const PinDef PowerOff = { .pinNumber = GPIO_PIN_8, .pinPort = GPIOA };

static const PinDef ADC = { .pinNumber = GPIO_PIN_0, .pinPort = GPIOB };

static const PinDef HB = { .pinNumber = GPIO_PIN_3, .pinPort = GPIOB };