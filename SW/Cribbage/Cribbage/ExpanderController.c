#include "ExpanderController.h"
#include <stm32g4xx_hal.h>
#include "I2C.h"

#define NUM_BANKS_READ 2

typedef struct 
{
	TCA9555 tcaDevice;
	uint8_t P0Bank[NUM_BANKS_READ];
	uint8_t P1Bank[NUM_BANKS_READ];
}Expanders;

Expanders expanders[NUMBER_EXPANDERS];

// P00 - OUT0
// P01 - OUT1
// P02 - OUT2
// P03 - OUT3
// P04 - OUT4
// P05 - OUT5
// P06 - OUT6
// P07 - OUT7
// P10 - OUT9
// P11 - OUT8
// P12 - B1 
// P16 - B0

static void InitTCADevice()
{
	static const uint8_t DEFAULT_PINS_OFF = 0x44; // 01000100 = 0x44
	for (int i = 0; i < NUMBER_EXPANDERS; i++)
	{
		//First step, ensure configurations are set correctly.
		//Bank 0 is all inputs
		configPort(&expanders[i].tcaDevice, Bank_0, 0xff);
		// Bank 1 has P12, P16 as outputs, P10, )11 as inputs. Set all to inputs except for the two outputs.
		configPort(&expanders[i].tcaDevice, Bank_1, 0xff & ~(1 << 2 | 1 << 6));
		
		//Next, disable the b0, and b1 rails., inverted logic.
		writeBank(&expanders[i].tcaDevice, Bank_1, DEFAULT_PINS_OFF);
	}
}

//
// GetBankReadings interprets the results of a prior read. Interprets the 20 bits that were read into
// the 5 Least significant bits. 0b000000RRRRRRRRRR000000LLLLLLLLLL
//
uint32_t GetBankReadings(uint8_t TCADevice)
{
	uint32_t retVal = 0;
	uint8_t P0Bank0 = (expanders[TCADevice].P0Bank[0]);
	uint8_t P0Bank1 = (expanders[TCADevice].P0Bank[1]);
	uint8_t P1Bank0 = (expanders[TCADevice].P1Bank[0]);
	uint8_t P1Bank1 = (expanders[TCADevice].P1Bank[1]);
	// Low- Left
	retVal |= (P1Bank1 & (1 << 0)) == 0 ? (1 << 0) : 0; 
	retVal |= (P0Bank1 & (1 << 7)) == 0 ? (1 << 1) : 0; 
	retVal |= (P0Bank1 & (1 << 5)) == 0 ? (1 << 2) : 0; 
	retVal |= (P0Bank1 & (1 << 3)) == 0 ? (1 << 3) : 0; 
	retVal |= (P0Bank1 & (1 << 0)) == 0 ? (1 << 4) : 0; 
								   
	// High Left				   
	retVal |= (P0Bank0 & (1 << 1)) == 0 ? (1 << 5) : 0; 
	retVal |= (P0Bank0 & (1 << 2)) == 0 ? (1 << 6) : 0; 
	retVal |= (P0Bank0 & (1 << 4)) == 0 ? (1 << 7) : 0; 
	retVal |= (P0Bank0 & (1 << 6)) == 0 ? (1 << 8) : 0; 
	retVal |= (P1Bank0 & (1 << 1)) == 0 ? (1 << 9) : 0; 
								   
	// Low Right				   
	retVal |= (P1Bank1 & (1 << 1)) == 0 ? (1 << 16) : 0; 
	retVal |= (P0Bank1 & (1 << 6)) == 0 ? (1 << 17) : 0; 
	retVal |= (P0Bank1 & (1 << 4)) == 0 ? (1 << 18) : 0;
	retVal |= (P0Bank1 & (1 << 2)) == 0 ? (1 << 19) : 0;
	retVal |= (P0Bank1 & (1 << 1)) == 0 ? (1 << 20) : 0;
								   
	// High Right				   
	retVal |= (P0Bank0 & (1 << 0)) == 0 ? (1 << 21) : 0; 
	retVal |= (P0Bank0 & (1 << 3)) == 0 ? (1 << 22) : 0; 
	retVal |= (P0Bank0 & (1 << 5)) == 0 ? (1 << 23) : 0;
	retVal |= (P0Bank0 & (1 << 7)) == 0 ? (1 << 24) : 0;
	retVal |= (P1Bank0 & (1 << 0)) == 0 ? (1 << 25) : 0;
	return retVal;
}

//
// Sets up the chip for reading,
//
void StartBankReadings()
{
	TCAError err;
	SetB0s();
	HAL_Delay(1);
	for (uint8_t i = 0; i < NUMBER_EXPANDERS; i++)
	{
		err = readBank(&expanders[i].tcaDevice, Bank_0, &expanders[i].P0Bank[0]);
		err = readBank(&expanders[i].tcaDevice, Bank_1, &expanders[i].P1Bank[0]);
	}
	SetB1s();
	HAL_Delay(1);
	for (uint8_t i = 0; i < NUMBER_EXPANDERS; i++)
	{
		err = readBank(&expanders[i].tcaDevice, Bank_0, &expanders[i].P0Bank[1]);
		err = readBank(&expanders[i].tcaDevice, Bank_1, &expanders[i].P1Bank[1]);
	}
}

void InitExpanders()
{
	InitializeI2C();
	for (int i = 0; i < NUMBER_EXPANDERS; i++)
	{
		expanders[i].tcaDevice.addr =  (DEFAULT_TCA_9555_ADDR + i) << 1;
	}
	InitTCADevice();
}

// OE1/2 for the buffer, are negative logic.

static void TurnB0(uint8_t TCADevice)
{
	static const uint8_t enableB0Config = 0x04; // 00000100 = 0x04
	writeBank(&expanders[TCADevice].tcaDevice, Bank_1, enableB0Config);
}

static void TurnB1(uint8_t TCADevice)
{
	static const uint8_t enableB1Config = 0x40; // 010000000 = 0x40
	writeBank(&expanders[TCADevice].tcaDevice, Bank_1, enableB1Config);
}

void SetB0s()
{
	for (uint8_t i = 0; i < NUMBER_EXPANDERS; i++)
	{
		TurnB0(i);
	}
}

void SetB1s()
{
	for (uint8_t i = 0; i < NUMBER_EXPANDERS; i++)
	{
		TurnB1(i);
	}
}
