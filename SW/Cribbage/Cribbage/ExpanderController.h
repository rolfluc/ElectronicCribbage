#pragma once
#include "TCA9555.h"

#define NUMBER_EXPANDERS 6
void InitExpanders();
uint16_t GetBankReadings(uint8_t TCADevice);
void StartBankReading(uint8_t TCADevice);
void SetUse3v2(uint8_t TCADevice, bool doUse);