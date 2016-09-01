#pragma once

#include "Console.hpp"
using namespace std;

void printByteToHex(BYTE buf[], const unsigned int &packetSize) {
	char strBuffer[32];
	int length = 0;
	for (int i = 0; i < 2; i++)
	{
		length += sprintf(strBuffer + length, "%02X", (unsigned char)buf[i]);
	}
	Console::setColor(14);
	Console::write("%s", strBuffer);
	Console::setColor(7);

	if (packetSize <= 2) {
		Console::writeLine(" ");
		return;
	}

	char strBuffer2[4096];
	length = 0;
	for (int i = 2; i < packetSize; i++)
	{
		length += sprintf(strBuffer2 + length, "%02X", (unsigned char)buf[i]);
	}
	Console::writeLine("%s", strBuffer2);
}

// TODO need refactor this function
void readDump(const DWORD &addrInDumpPacket, const unsigned int &packetSize) {
	BYTE buf[4096];
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)addrInDumpPacket, buf, packetSize, NULL);
	printByteToHex(buf, packetSize);
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
