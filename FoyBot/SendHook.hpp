#pragma once

#include "Functions.h"
#include "Memory.h"
#include "PacketUtils.hpp"


class SendHook
{
private:

public:
	static DWORD jumpBackSend;
	static DWORD stackElementNotUsed;
	static unsigned int packetSize;
	static DWORD addrInDumpPacket;
	static void readPacketBeforeSendHook();

	static DWORD sendFunctionAddr;
	static LPBYTE ptrToDetermine;
	static void sendPacket(LPBYTE Buffer, unsigned int packetSize);

private:
	static void processSendPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize);

};

DWORD SendHook::sendFunctionAddr = 0;
LPBYTE SendHook::ptrToDetermine = nullptr;
inline void SendHook::sendPacket(LPBYTE Buffer, unsigned int packetSize)
{
	Sleep(150);
	ptrToDetermine = LPBYTE(0xC74900); // la valeur de ECX au moment du send

	__asm
	{
		MOV ECX, ptrToDetermine
		PUSH Buffer
		PUSH packetSize
		CALL sendFunctionAddr
	}
}

DWORD SendHook::jumpBackSend = 0;
DWORD SendHook::stackElementNotUsed = 0;
unsigned int SendHook::packetSize = 0;
DWORD SendHook::addrInDumpPacket = 0;

inline void SendHook::processSendPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize) {
	Console::setColor(7);
	Console::write("[Send]");
	BYTE bytes[4096];
	readDump(addrInDumpPacket, packetSize, bytes);

	auto packetHeader = bytes[0] << 8 | bytes[1];
	switch (packetHeader)
	{
	case 0xF300:
		Console::write("[Chat] ");
		printByteToChar(bytes, packetSize);
		return;
	case 0x3704:
	{
		Console::write("[Walk]");

		auto pos = PacketUtils::computeCoord(bytes[2], bytes[3], bytes[4]);
		uint16_t posX = pos->getX();
		uint16_t posY = pos->getY();

		Console::write(" X=%d Y=%d ", posX, posY);

		// Packet example : 
		//3704 259 0E 0
		//259 / 4 = x pos
		//0E + 259 % 4 = y pos

		Console::write("\n");
		return;
	}
	case 0x6A09:
	{
		Console::write("[Hover] ");
		// Example
		// 9A09 ED7D8E06 00
		//      ED7D8E06 = ID

		auto ID = bytes[2] << 24 | bytes[3] << 16 | bytes[4] << 8 | bytes[5];
		Console::write("ID=%08X \n", ID);
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


inline __declspec(naked) void SendHook::readPacketBeforeSendHook()
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