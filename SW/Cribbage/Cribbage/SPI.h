#pragma once
#include "stdint.h"
void InitSPI();
void SendDisplay(uint8_t* buffer, uint32_t cnt);