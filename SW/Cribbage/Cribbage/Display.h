#pragma once
#include "stdint.h"
#include "Color.h"
#include "Segment.h"
void RunDisplayStateMachine();
void SetUserText(Color c, char* dat, uint8_t len);
void SetSystemText(Color c, char* dat, uint8_t len);