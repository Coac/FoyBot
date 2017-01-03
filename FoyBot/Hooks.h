#pragma once

#include "Functions.h"
#include "Memory.h"

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


