#pragma once

#include "Console.h"
using namespace std;
void readDump(DWORD addrInDumpPacket, DWORD packetSize) {
	Console::write("Size:%02x Addr:%02x Packet: ", packetSize, addrInDumpPacket);

	char buf[100];
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)addrInDumpPacket, buf, packetSize, NULL);

	char strBuffer[10];
	int length = 0;
	for (int i = 0; i < 2; i++)
	{
		length += sprintf(strBuffer + length, "%02X", (unsigned char)buf[i]);
	}
	Console::setColor(14);
	Console::write("%s", strBuffer);
	Console::setColor(7);


	char strBuffer2[500];
	length = 0;
	for (int i = 2; i < packetSize; i++)
	{
		length += sprintf(strBuffer2 + length, "%02X", (unsigned char)buf[i]);
	}
	Console::writeLine("%s", strBuffer2);

}

LPBYTE allocateMem(LPCVOID _buffer, SIZE_T size) {

	LPBYTE allocatedMemory = (LPBYTE)VirtualAllocEx(GetCurrentProcess(), NULL, 0x100, MEM_COMMIT, PAGE_READWRITE);
	bool suces = WriteProcessMemory(GetCurrentProcess(), (LPVOID)allocatedMemory, _buffer, size, NULL);

	if (suces) {
		Console::writeLine("Sucess Allocating");
	}
	else {
		Console::writeLine("Error Allocating");
	}

	return allocatedMemory;
}
