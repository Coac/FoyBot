#include "Functions.h"
#include "Memory.h"


void processSendPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize) {
	Console::write("[Send]");
	BYTE bytes[4096];
	readDump(addrInDumpPacket, packetSize, bytes);

	int packetHeader = bytes[0] << 8 | bytes[1] ;
	switch (packetHeader)
	{
	case 0xF300:
		Console::write("[Chat]");
		break;
	default:
		break;
	}
	Console::write(" ");

	Console::write("Size:%02x Addr:%02x Packet: ", packetSize, addrInDumpPacket);
	printByteToHex(bytes, packetSize);
}


DWORD TestJumpBack = 0;
DWORD stackElementNotUsed = 0;
unsigned int packetSize = 0;
DWORD addrInDumpPacket = 0;
__declspec(naked) void readPacketBeforeSendHook()
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




void processRecvPacket(const DWORD &addrInDumpPacket, unsigned int  &packetSize) {
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
	default:
		break;
	}
	Console::write(" ");

	Console::write("Size:%02x Addr:%02x Packet: ", packetSize, addrInDumpPacket);
	printByteToHex(bytes, packetSize);
	Console::setColor(7);
}


unsigned int recvPacketSize = 0;
DWORD recvAddrDump = 0;
DWORD jumpBackRecv = 0;
__declspec(naked) void readPacketRecv()
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


DWORD sendFunctionAddr = 0;
LPBYTE ptrToDetermine = 0;

void Send_To_Server(LPBYTE Buffer, unsigned int packetSize)
{
	Sleep(150);
	ptrToDetermine = (LPBYTE)0xC74900; // la valeur de ECX au moment du send

	__asm
	{
		MOV ECX, ptrToDetermine
		PUSH Buffer
		PUSH packetSize
		CALL sendFunctionAddr
	}
}


