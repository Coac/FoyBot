#pragma once

#include "Hooks.h" // To remove and incorporate code here


class SendHook
{
private:

public:
	static void readPacketBeforeSendHook();

	static DWORD TestJumpBack;
	static DWORD stackElementNotUsed;
	static unsigned int packetSize;
	static DWORD addrInDumpPacket;
};

DWORD SendHook::TestJumpBack = 0;
DWORD SendHook::stackElementNotUsed = 0;
unsigned int SendHook::packetSize = 0;
DWORD SendHook::addrInDumpPacket = 0;

// TODO : to move
void processSendPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize) {
	Console::write("[Send]");
	BYTE bytes[4096];
	readDump(addrInDumpPacket, packetSize, bytes);

	int packetHeader = bytes[0] << 8 | bytes[1];
	switch (packetHeader)
	{
	case 0xF300:
		Console::write("[Chat]");
		break;
	case 0x3704:
		Console::write("[Walk]");
		//3704 259 0E 0
		//259 / 4 = x pos
		//0E + 259 % 4 = y pos
		break;
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

	__asm jmp[TestJumpBack]
}