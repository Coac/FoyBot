#pragma once

#include "Console.hpp"
#include "SendHook.hpp"
#include"StringUtils.h"

class Actions
{
private:

public:
	static void walkXY(unsigned int x, unsigned int y);
};

inline void Actions::walkXY(unsigned int x, unsigned int y)
{
	char packetStr[11];

	uint16_t posX = x * 4 + y/256;
	uint8_t posY = y % 256;

	sprintf(packetStr, "3704");
	sprintf(packetStr + 4, "%03X", posX);
	sprintf(packetStr + 7, "%02X", posY);
	sprintf(packetStr + 9, "0");


	char buffer[11];
	auto size = hex2bin(packetStr, buffer);
	SendHook::sendPacket(LPBYTE(buffer), size);
}
