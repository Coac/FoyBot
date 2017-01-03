#pragma once

#include "Functions.h"
#include "Memory.h"


class SendHook
{
private:

public:
	static void readPacketBeforeSendHook();
	static void processSendPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize);

	static DWORD jumpBackSend;
	static DWORD stackElementNotUsed;
	static unsigned int packetSize;
	static DWORD addrInDumpPacket;
};

DWORD SendHook::jumpBackSend = 0;
DWORD SendHook::stackElementNotUsed = 0;
unsigned int SendHook::packetSize = 0;
DWORD SendHook::addrInDumpPacket = 0;

void SendHook::processSendPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize) {
	Console::setColor(7);
	Console::write("[Send]");
	BYTE bytes[4096];
	readDump(addrInDumpPacket, packetSize, bytes);

	int packetHeader = bytes[0] << 8 | bytes[1];
	switch (packetHeader)
	{
	case 0xF300:
		Console::write("[Chat]\n");
		return;
	case 0x3704:
	{
		Console::write("[Walk]");

		BYTE high = bytes[3] >> 4; // Get the 4 high bits
		uint16_t factor4PosX = (bytes[2] << 4 | high);
		uint16_t posX = factor4PosX / 4;

		BYTE lowPosY = bytes[3] & 0x0F; // Get the 4 low bits
		BYTE highPosY = bytes[4] >> 4;
		uint16_t posY = lowPosY << 4 | highPosY;
		posY += (factor4PosX % 4) * 255;

		Console::write(" X=%d Y=%d ", posX, posY);

		// Packet example : 
		//3704 259 0E 0
		//259 / 4 = x pos
		//0E + 259 % 4 = y pos

		Console::write("\n");
		return;
	}
	case 0x5F03:
		Console::write("[HeartBeat]\n");
		return;
	default:
		break;
	}
	Console::write(" ");

	Console::write("Size:%02x Addr:%02x Packet: ", packetSize, addrInDumpPacket);
	printByteToHex(bytes, packetSize);
}


__declspec(naked) void SendHook::readPacketBeforeSendHook()
{
	__asm {

		POP stackElementNotUsed
		POP packetSize
		POP addrInDumpPacket
		PUSH addrInDumpPacket
		PUSH packetSize
		PUSH stackElementNotUsed

		PUSH EBP
		MOV EBP, ESP
		PUSH ESI
		MOV ESI, ECX

	}

	processSendPacket(addrInDumpPacket, packetSize);

	__asm jmp[jumpBackSend]
}