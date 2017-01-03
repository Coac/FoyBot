#pragma once

#include "Functions.h"
#include "Memory.h"
#include "PacketUtils.hpp"

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
		Console::write("[Chat] ");
		printByteToChar(bytes, packetSize);
		return;
	case 0x8700:
		Console::write("[Walk]\n");
		return;
	case 0x9500: case 0x9A00: case 0xC301:
		Console::write("[Announce] ");
		printByteToChar(bytes, packetSize);
		return;
	case 0xF301: case 0x7F00:
		Console::write("[HeartBeat]\n");
		return;
	case 0x8000:
	{
		Console::write("[EntityDisappear] ");
		// Example
		// 8000 ED7D8E06 00
		//      ED7D8E06 = NPC_ID

		int NPC_ID = bytes[2] << 24 | bytes[3] << 16 | bytes[4] << 8 | bytes[5];
		Console::write("ID=%08X", NPC_ID);
	}
	break;
	case 0x8600:
	{
		Console::write("[EntityMove] ");
		// Example
		// 8600 74981E00 288A5 288A2 888F443017
		//      74981E00 = NPC_ID
		//               288A5 = initialPos
		//                     288A2 = destPos

		int NPC_ID = bytes[2] << 24 | bytes[3] << 16 | bytes[4] << 8 | bytes[5];
		Console::write("ID=%08X", NPC_ID);

		// Position
		int* pos = PacketUtils::computeCoord(bytes[6], bytes[7], bytes[8]);
		uint16_t posX = pos[0];
		uint16_t posY = pos[1];
		Console::write(" intial : X=%d Y=%d ", posX, posY);

		pos = PacketUtils::computeCoord(
			((bytes[8] & 0x0F) << 4) | (bytes[9] >> 4), 
			((bytes[9] & 0x0F) << 4) | (bytes[10] >> 4),
			((bytes[10] & 0x0F) << 4) | (bytes[11] >> 4));
		posX = pos[0];
		posY = pos[1];
		Console::write(" dest : X=%d Y=%d ", posX, posY);
	}
	break;
	case 0x5708:
	{
		Console::write("[EntityAppear] ");
		// Example
		// 5708 570006 CF7D8E06 C8000000000000000000740000000000000000000000000000000000000000000000000000000000000000000000 290 7D 400000000000000 4B6166726120566F74696E6720537461666623707274
		//          CF7D8E06 = NPC_ID                                                                                        290/4 = x                Name
		//																													7D = y

		// NPC_ID
		int NPC_ID = bytes[5] << 24 | bytes[6] << 16 | bytes[7] << 8 | bytes[8];
		Console::write("ID=%08X", NPC_ID);

		// Position
		int* pos = PacketUtils::computeCoord(bytes[55], bytes[56], bytes[57]);
		uint16_t posX = pos[0];
		uint16_t posY = pos[1];
		Console::write(" X=%d Y=%d ", posX, posY);
		
		// Name
		Console::write("Name=");
		for (int i = 65; i < packetSize; i++)
		{
			Console::write("%c", bytes[i]);
		}

	}
		break;
	default:
		break;
	}
	Console::write(" ");

	Console::write("Size:%02x Addr:%02x Packet: ", packetSize, addrInDumpPacket);
	printByteToHex(bytes, packetSize);
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