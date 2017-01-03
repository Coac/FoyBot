#pragma once

#include <Windows.h>
#include <iostream> 
#include "Store/Coord.hpp"

class PacketUtils
{
private:

public:
	static Coord* computeCoord(BYTE byte1, BYTE byte2, BYTE byte3);
};

Coord* PacketUtils::computeCoord(BYTE byte1, BYTE byte2, BYTE byte3) {
	BYTE high = byte2 >> 4; // Get the 4 high bits
	uint16_t factor4PosX = (byte1 << 4 | high);
	uint16_t posX = factor4PosX / 4;

	BYTE lowPosY = byte2 & 0x0F; // Get the 4 low bits
	BYTE highPosY = byte3 >> 4;
	uint16_t posY = lowPosY << 4 | highPosY;
	posY += (factor4PosX % 4) * 255;

	return new Coord(posX, posY);
}
