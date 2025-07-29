#pragma once
#include <stdint.h>
void InitializePowerPin();
void InitPowerSense();
void DetermineSufficientVoltage(uint16_t counts);
void HandleButtonPress();