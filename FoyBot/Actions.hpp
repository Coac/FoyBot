#pragma once

#include "Console.hpp"
#include "SendHook.hpp"
#include"StringUtils.h"

class Actions
{
private:

public:
	static void walkXY(unsigned int x, unsigned int y);
	static void attack(unsigned int id);
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

inline void Actions::attack(unsigned int id)
{
	char packetStr[15];
	sprintf(packetStr, "6903");
	sprintf(packetStr + 4, "%08X", id);
	sprintf(packetStr + 12, "07");

	Console::writeLine("%s", packetStr);

	char buffer[15];
	auto size = hex2bin(packetStr, buffer);
	SendHook::sendPacket(LPBYTE(buffer), size);
}