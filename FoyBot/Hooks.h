#include "Functions.h"
#include "Memory.h"




DWORD TestJumpBack = 0;
DWORD stackElementNotUsed = 0;
DWORD packetSize = 0;
DWORD addrInDumpPacket = 0;
__declspec(naked) void testHook()
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

	readDump(addrInDumpPacket, packetSize);


	__asm jmp[TestJumpBack]
}


DWORD sendFunctionAddr = 0;
LPBYTE ptrToDetermine = 0;

void Send_To_Server(LPBYTE Buffer)
{
	Sleep(150);
	ptrToDetermine = (LPBYTE)0xC74900; // la valeur de ECX au moment du send

	__asm
	{
		MOV ECX, ptrToDetermine
		PUSH Buffer
		PUSH 8
		CALL sendFunctionAddr
	}
}


