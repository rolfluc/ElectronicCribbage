#include "ExpanderController.h"
#include <stm32c0xx_hal.h>
#include "I2C.h"

typedef struct 
{
	TCA9555 tcaDevice;
	uint8_t P0Bank;
	uint8_t P1Bank;
	bool use3V2;
}Expanders;

Expanders expanders[NUMBER_EXPANDERS];

typedef enum 
{
	Rail_3v2 = TCAPin_16,
	Rail_2v7 = TCAPin_06,
	Rail_B0 = TCAPin_07,
	Rail_B1 = TCAPin_17,
}ExpanderPins;

static void InitTCADevice()
{
	for (int i = 0; i < NUMBER_EXPANDERS; i++)
	{
		//P02,3,4,5 and P10,1,2,3,4,5 are inputs, P06,7 and P16,7 are outputs.
		//First step, ensure configurations are set correctly.
		// x,x,b11,b12,b13,b14,2v7,b0_en
		configPort(&expanders[i].tcaDevice, Bank_0, 0b00111111);
		// b10,b04,b03,b02,b01,b00,3v2,b1_en
		configPort(&expanders[i].tcaDevice, Bank_1, 0b00111111);
		
		//Next, disable the 3v2, 2v7, b0, and b1 rails.
		writePin(&expanders[i].tcaDevice, (TCA9555Pins)Rail_3v2, false);
		writePin(&expanders[i].tcaDevice, (TCA9555Pins)Rail_2v7, false);
		writePin(&expanders[i].tcaDevice, (TCA9555Pins)Rail_B0, false);
		writePin(&expanders[i].tcaDevice, (TCA9555Pins)Rail_B1, false);
	}
}

//
// GetBankReadings interprets the results of a prior read.
// to map to contiguous. e.g. P10 is B10, the 5th bit, not the 9th.
//
uint16_t GetBankReadings(uint8_t TCADevice)
{
	uint16_t retVal = 0;
	uint8_t* P0Bank = &(expanders[TCADevice].P0Bank);
	uint8_t* P1Bank = &(expanders[TCADevice].P1Bank);
	
	retVal |= (1 << 0) & (*P1Bank & (1 << 5)); // B00 - P15
	retVal |= (1 << 1) & (*P1Bank & (1 << 4)); // B01 - P14
	retVal |= (1 << 2) & (*P1Bank & (1 << 3)); // B02 - P13
	retVal |= (1 << 3) & (*P1Bank & (1 << 2)); // B03 - P12
	retVal |= (1 << 4) & (*P1Bank & (1 << 1)); // B04 - P11
	
	retVal |= (1 << 8) & (*P1Bank & (1 << 0)); // B10 - P10
	retVal |= (1 << 9) & (*P0Bank & (1 << 2)); // B11 - P02
	retVal |= (1 <<10) & (*P0Bank & (1 << 3)); // B12 - P03
	retVal |= (1 <<11) & (*P0Bank & (1 << 4)); // B13 - P04
	retVal |= (1 <<12) & (*P0Bank & (1 << 5)); // B14 - P05
	return retVal;
}

//
// Sets up the chip for reading,
//
void StartBankReading(uint8_t TCADevice)
{
	TCAError err;
	TCA9555Pins targetpin = expanders[TCADevice].use3V2 ? (TCA9555Pins)Rail_3v2 : (TCA9555Pins)Rail_2v7;
	err = writePin(&expanders[TCADevice].tcaDevice, targetpin, true);
	err = writePin(&expanders[TCADevice].tcaDevice, (TCA9555Pins)Rail_B0, true);
	// TODO sleep. Need EE measurements here to determine how long.
	HAL_Delay(1);
	err = readBank(&expanders[TCADevice].tcaDevice, Bank_0, &expanders[TCADevice].P0Bank);
	err = writePin(&expanders[TCADevice].tcaDevice, (TCA9555Pins)Rail_B0, false);
	// TODO sleep. Need EE measurements here to determine how long.
	HAL_Delay(1);
	err = writePin(&expanders[TCADevice].tcaDevice, (TCA9555Pins)Rail_B1, true);
	// TODO sleep. Need EE measurements here to determine how long.
	HAL_Delay(1);
	err = readBank(&expanders[TCADevice].tcaDevice, Bank_1, &expanders[TCADevice].P1Bank);
	err = writePin(&expanders[TCADevice].tcaDevice, (TCA9555Pins)Rail_B1, false);
	err = writePin(&expanders[TCADevice].tcaDevice, targetpin, false);
}

void InitExpanders()
{
	InitializeI2C();
	for (int i = 0; i < NUMBER_EXPANDERS; i++)
	{
		expanders[i].tcaDevice.addr = DEFAULT_TCA_9555_ADDR + i;
		
		// By definition, these are unknown.
		//tcaDevices[i].shadowRegisters[InputPort0] = 0;
		//tcaDevices[i].shadowRegisters[InputPort1] = 0;
		expanders[i].tcaDevice.shadowRegisters[OutputPort0] = 0xff;
		expanders[i].tcaDevice.shadowRegisters[OutputPort1] = 0xff;
		expanders[i].tcaDevice.shadowRegisters[PolarityInversionPort0] = 0x00;
		expanders[i].tcaDevice.shadowRegisters[PolarityInversionPort1] = 0x00;
		expanders[i].tcaDevice.shadowRegisters[ConfigPort0] = 0xff;
		expanders[i].tcaDevice.shadowRegisters[ConfigPort1] = 0xff;
	}
	InitTCADevice();
}

void SetUse3v2(uint8_t TCADevice, bool doUse)
{
	expanders[TCADevice].use3V2 = doUse;
}

void TurnB0(uint8_t TCADevice, bool turnOn)
{
	writePin(&expanders[TCADevice].tcaDevice, (TCA9555Pins)Rail_B0, turnOn);
}

void TurnB1(uint8_t TCADevice, bool turnOn)
{
	writePin(&expanders[TCADevice].tcaDevice, (TCA9555Pins)Rail_B1, turnOn);
}