#pragma once

#include "Functions.h"
#include "Memory.h"

class RecvHook
{
private:

public:
	static void readPacketRecv();
	static void processRecvPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize);

	static unsigned int recvPacketSize;
	static DWORD recvAddrDump;
	static DWORD jumpBackRecv;
};

DWORD RecvHook::recvAddrDump = 0;
DWORD RecvHook::jumpBackRecv = 0;
unsigned int RecvHook::recvPacketSize = 0;

void RecvHook::processRecvPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize) {
	Console::setColor(8);
	Console::write("[Recv]");
	BYTE bytes[4096];
	readDump(addrInDumpPacket, packetSize, bytes);

	int packetHeader = bytes[0] << 8 | bytes[1];
	switch (packetHeader)
	{
	case 0x8E00:
		Console::write("[Chat]");
		break;
	case 0x8700:
		Console::write("[Walk]");
		break;
	default:
		break;
	}
	Console::write(" ");

	Console::write("Size:%02x Addr:%02x Packet: ", packetSize, addrInDumpPacket);
	printByteToHex(bytes, packetSize);
	Console::setColor(7);
}

__declspec(naked) void RecvHook::readPacketRecv()
{


	__asm {

		MOV recvPacketSize, EAX
		MOV recvAddrDump, EDX

		PUSH EDX
		PUSH EAX
		lea ecx, dword ptr ds : [esi + 0x40]

	}

	__asm {
		PUSH ESI
		PUSH EDI
		PUSH ECX
		PUSH EDI
		PUSH EBX
		PUSH EAX
	}

	processRecvPacket(recvAddrDump, recvPacketSize);

	__asm {
		POP EAX
		POP EBX
		POP EDI
		POP ECX
		POP EDI
		POP ESI
	}

	__asm jmp[jumpBackRecv]
}