#include "Display.h"
#include <stdbool.h>
#include "stm32g4xx_hal.h"
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 8
static Color systemColor = { 0 };
static uint8_t sysLen = 0;
static SegmentVal SystemBuffer[BUFFER_SIZE];

static Color userColor = { 0 };
static uint8_t userLen = 0;
static SegmentVal UserBuffer[BUFFER_SIZE];
static uint8_t bufferPtr = 0;
static bool IsUserActive = false;

static uint8_t ConvertStringToBuffer(SegmentVal* buffer, char* dat, uint8_t len) {
	uint8_t convertedLength = 0;
	if (len > BUFFER_SIZE) {
		// Just cut it short. Really shouldn't happen but keep safe.
		len = BUFFER_SIZE;
	}
	convertedLength = len;
	for (uint8_t i = 0; i < len; i++) {
		buffer[i] = GetSegmentForChar(dat[i]);
	}
	return convertedLength;
}

void SetUserText(Color c, char* dat, uint8_t len) {
	userColor = c;
	IsUserActive = true;
	userLen = ConvertStringToBuffer(&UserBuffer[0], dat, len);
}

static inline bool AreBuffersTheSame(SegmentVal* currentBuffer, SegmentVal* newBuffer, uint8_t newBufferLen) {
	for (uint8_t i = 0; i < newBufferLen; i++) {
		if (currentBuffer[i] != newBuffer[i]) {
			return false;
		}
	}
	return true;
}

void SetSystemText(Color c, char* dat, uint8_t len) {
	SegmentVal tmpBuffer[BUFFER_SIZE];
	uint8_t tmpLen = 0;
	// TODO compare if the same. If the same do nothing.
	// TODO if compared results are different, reset to zero bufferPtr
	tmpLen = ConvertStringToBuffer(&tmpBuffer[0], dat, len);
	// If the two buffers differ, make some changes.
	if (!AreBuffersTheSame(&SystemBuffer[0], &tmpBuffer[0], tmpLen)) {
		sysLen = tmpLen;
		memcpy(&SystemBuffer[0], &tmpBuffer[0], sysLen * sizeof(tmpBuffer[0]));
		systemColor = c;
		bufferPtr = 0;
	}
	// If they are the same, change nothing.
}

// Thought process on this state machine. First, if user is active, run through user data
// Once user data clears, revert back to system data. PegHandler will provide real time changes. 
// If no change from pegHandler has occured within a few seconds, rotate through the following:
// Current red delta (R), current green Delta (G), total red points (R), total green points (G).
// These should alternate through every 5s. Peg will provide immediate - i.e. delta between the two pegs
// for the respective player.
// User data as of now is defined as battery voltage. Dash replaces the decimal.

// This state machine is only responsible for managing user vs system Display, and timing of displaying
// strings. Peg Handler will manage how to display.

static const uint32_t UpdateRate_ms = 750;
static uint32_t lastUpdated = 0;
void RunDisplayStateMachine()
{
	uint32_t currentTime = HAL_GetTick();
	bool doIncrement = currentTime > (lastUpdated + UpdateRate_ms);
	Color desiredColor = ColorUndefined;
	SegmentVal v0 = Display_dash;
	SegmentVal v1 = Display_dash;

	if(IsUserActive) {
		desiredColor = userColor;
		v0 = UserBuffer[bufferPtr];
		v1 = UserBuffer[bufferPtr + 1]; 
		// For user data, we do not wrap. Display once and revert back to system.
		if (doIncrement) {
			lastUpdated = currentTime;
			if (bufferPtr == userLen - 1) {
				IsUserActive = false;
				bufferPtr = 0;
			} else {
				bufferPtr++;
			}
		}
	} else {
		desiredColor = systemColor;
		// Most of the time, it's just numeric.
		if (sysLen == 2)
		{
			v0 = SystemBuffer[bufferPtr];
			v1 = SystemBuffer[bufferPtr + 1]; 
		}
		else
		{
			if (bufferPtr == sysLen - 1) {
				v0 = SystemBuffer[sysLen - 1];
				v1 = SystemBuffer[0];
			}
			else {
				v0 = SystemBuffer[bufferPtr];
				v1 = SystemBuffer[bufferPtr + 1]; 
			}
			if (doIncrement) {
				lastUpdated = currentTime;
				bufferPtr = ((bufferPtr + 1) % sysLen);
			}
		}
		
	}
	
	SetDisplay(v0, desiredColor, v1, desiredColor);
}