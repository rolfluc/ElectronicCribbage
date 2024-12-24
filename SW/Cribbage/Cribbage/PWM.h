#pragma once
#include <stdint.h>

void InitTimer();
void SendPWMDMA(uint8_t* buffer, uint16_t count);