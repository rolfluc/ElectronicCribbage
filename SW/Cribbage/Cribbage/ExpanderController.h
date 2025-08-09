#pragma once
#include "TCA9555.h"

#define NUMBER_EXPANDERS 6 
void InitExpanders();
uint32_t GetBankReadings(uint8_t TCADevice);
void StartBankReadings();
void SetUse3v2(uint8_t TCADevice, bool doUse);
void InitOutputVoltages();
void SetB0s();
void SetB1s();