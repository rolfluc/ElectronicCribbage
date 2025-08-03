#include "TCA9555.h"
#include "I2C.h"


static HAL_StatusTypeDef WriteI2C(uint8_t addr, uint8_t* data, uint8_t length)
{
	return I2CWrite(addr, data, length);
}

static HAL_StatusTypeDef ReadI2C(uint8_t addr, uint8_t* data, uint8_t len)
{
	return I2CRead(addr, data, len);
}

static TCAError WriteRegister(uint8_t addr, TCACommandByte cmd, uint8_t data)
{
	uint8_t writeBufer[2] = {(uint8_t)cmd, data};
	HAL_StatusTypeDef err = WriteI2C(addr, writeBufer, sizeof(writeBufer));
	if (err != HAL_OK)
	{
		return Error_Fail;
	}
	return Error_Okay;
}

static TCAError ReadRegister(uint8_t deviceAddress, uint8_t memoryAddress, uint8_t* ret)
{
	uint8_t returnByte = 0;	
	HAL_StatusTypeDef err = I2CReadMemory(
		deviceAddress,
		memoryAddress,
		1,
		&returnByte,
		1
	);
	if (err != HAL_OK)
	{
		
		return Error_Fail;
	}
	*ret = returnByte;
	return Error_Okay;
}

static inline uint8_t PinToOffset(TCA9555Pins pin)
{
	switch (pin)
	{
	case TCAPin_00:
	case TCAPin_10:
		{
			return 1 << 0;
		}
	case TCAPin_01:
	case TCAPin_11:
		{
			return 1 << 1;
		}
	case TCAPin_02:
	case TCAPin_12:
		{
			return 1 << 2;
		}
	case TCAPin_03:
	case TCAPin_13:
		{
			return 1 << 3;
		}
	case TCAPin_04:
	case TCAPin_14:
		{
			return 1 << 4;
		}
	case TCAPin_05:
	case TCAPin_15:
		{
			return 1 << 5;
		}
	case TCAPin_06:
	case TCAPin_16:
		{
			return 1 << 6;
		}
	case TCAPin_07:
	case TCAPin_17:
		{
			return 1 << 7;
		}
	default:
		break;
	}
}

TCAError readPin(TCA9555* dev, TCA9555Pins pin, bool* retVal)
{
	TCAError err = Error_Okay;
	return err;
}

TCAError writePin(TCA9555* dev, TCA9555Pins pin, bool writeValHigh)
{
	TCAError err = Error_Okay;
	TCACommandByte cmd = pin < TCAPin_10 ? OutputPort0 : OutputPort1;
	if (writeValHigh)
	{
		dev->shadowRegisters[cmd] |=  PinToOffset(pin);
	}
	else
	{
		dev->shadowRegisters[cmd]  &= ~PinToOffset(pin);
	}
	
	err = WriteRegister(dev->addr, cmd, dev->shadowRegisters[cmd]);
	return err;
}

TCAError readBank(TCA9555* dev, TCABank bank, uint8_t* retVal)
{
	TCAError err = Error_Okay;
	// TODO may need to modify the returnValue here.
	ReadRegister(dev->addr, (uint8_t)bank, retVal);
	return err;
}

TCAError writeBank(TCA9555* dev, TCABank bank, uint8_t writeVal)
{
	TCAError err = Error_Okay;
	TCACommandByte reg = (bank == Bank_0) ? OutputPort0 : OutputPort1;
	dev->shadowRegisters[reg] = writeVal;
	err = WriteRegister(dev->addr, reg, dev->shadowRegisters[reg]);
	return err;
}

TCAError configPin(TCA9555* dev, TCA9555Pins pin, TCAPinConfig config)
{
	TCAError err = Error_Okay;
	TCACommandByte cmd = pin < TCAPin_10 ? ConfigPort0 : ConfigPort1;
	if (config == Config_Input)
	{
		dev->shadowRegisters[cmd] |=  PinToOffset(pin);
	}
	else
	{
		dev->shadowRegisters[cmd]  &= ~PinToOffset(pin);
	}
	
	err = WriteRegister(dev->addr, cmd, dev->shadowRegisters[cmd]);
	return err;
}

TCAError configPort(TCA9555* dev, TCABank bank, uint8_t config)
{
	TCAError err = Error_Okay;
	TCACommandByte reg = (bank == Bank_0) ? ConfigPort0 : ConfigPort1;
	dev->shadowRegisters[(uint8_t)reg] = config;
	err = WriteRegister(dev->addr, reg, dev->shadowRegisters[(uint8_t)reg]);
	return err;
}
