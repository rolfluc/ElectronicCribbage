#pragma once
#include <stm32g4xx.h>
void InitializeI2C();

HAL_StatusTypeDef I2CWrite(uint16_t addr, uint8_t* dat, uint8_t length);
HAL_StatusTypeDef I2CRead(uint16_t addr, uint8_t* dat, uint8_t length);
HAL_StatusTypeDef I2CReadRegisters(uint16_t i2CAddress, uint8_t* writeDat, uint8_t writeLength, uint8_t* readDat, uint8_t readLength); 
HAL_StatusTypeDef I2CReadMemory(uint8_t deviceAddr, uint8_t memoryAddr, uint8_t memorySizeBytes, uint8_t* buffer, uint8_t countToRead);