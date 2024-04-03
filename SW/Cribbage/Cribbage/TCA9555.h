#pragma once
#include <stdint.h>
#include <stdbool.h>

#define DEFAULT_TCA_9555_ADDR 0x20
// Datasheet : https://www.ti.com/lit/ds/symlink/tca9555.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe&ts=1711990743015&ref_url=https%253A%252F%252Fwww.mouser.com%252F

typedef enum
{
	Error_Okay = 0,
	Error_Fail
}TCAError;

typedef enum
{
	Config_Output,
	Config_Input,
}TCAPinConfig;

typedef enum
{
	TCAPin_00 = 0,
	TCAPin_01,
	TCAPin_02,
	TCAPin_03,
	TCAPin_04,
	TCAPin_05,
	TCAPin_06,
	TCAPin_07,
	TCAPin_10,
	TCAPin_11,
	TCAPin_12,
	TCAPin_13,
	TCAPin_14,
	TCAPin_15,
	TCAPin_16,
	TCAPin_17,
}TCA9555Pins;

typedef enum 
{
	Bank_0 = 0,
	Bank_1,
}TCABank;

typedef enum
{
	InputPort0 = 0x00,
	InputPort1 = 0x01,
	OutputPort0 = 0x02,
	OutputPort1 = 0x03,
	PolarityInversionPort0 = 0x04,
	PolarityInversionPort1 = 0x05,
	ConfigPort0 = 0x06,
	ConfigPort1 = 0x07,
	RegisterCount,
}TCACommandByte;


typedef struct 
{
	uint8_t addr;
	uint8_t shadowRegisters[RegisterCount];
}TCA9555;

TCAError readPin(TCA9555* dev, TCA9555Pins pin, bool* retVal);
TCAError writePin(TCA9555* dev, TCA9555Pins pin, bool writeValHigh);
TCAError readBank(TCA9555* dev, TCABank bank, uint8_t* retVal);
TCAError writeBank(TCA9555* dev, TCABank bank, uint8_t writeVal);
TCAError writeAll(TCA9555* dev, uint16_t writeVal);
TCAError configPin(TCA9555* dev, TCA9555Pins pin, TCAPinConfig config);
TCAError configPort(TCA9555* dev, TCABank bank, uint8_t config);