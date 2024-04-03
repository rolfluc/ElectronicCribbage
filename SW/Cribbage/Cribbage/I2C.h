#pragma once
#include <stm32c0xx.h>
void InitializeI2C();

HAL_StatusTypeDef I2CWrite(uint16_t addr, uint8_t* dat, uint8_t length);
HAL_StatusTypeDef I2CRead(uint16_t addr, uint8_t* dat, uint8_t length);
HAL_StatusTypeDef I2CReadRegisters(uint16_t i2CAddress, uint8_t* writeDat, uint8_t writeLength, uint8_t* readDat, uint8_t readLength); 