#pragma once

#include "Console.hpp"
using namespace std;

inline void printByteToHex(BYTE buf[], const unsigned int &packetSize) {
	char strBuffer[32];
	auto length = 0;
	for (auto i = 0; i < 2; i++)
	{
		length += sprintf(strBuffer + length, "%02X", static_cast<unsigned char>(buf[i]));
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
	for (auto i = 2; i < packetSize; i++)
	{
		length += sprintf(strBuffer2 + length, "%02X", static_cast<unsigned char>(buf[i]));
	}
	Console::writeLine("%s", strBuffer2);
}

inline void printByteToChar(BYTE buf[], const unsigned int &packetSize) {
	for (auto i = 0; i < packetSize; i++)
	{
		Console::write("%c", buf[i]);
	}
	Console::writeLine("");
}

// out BYTE buffer[]
inline BYTE* readDump(const DWORD &addrInDumpPacket, const unsigned int &packetSize, BYTE buffer[]) {
	ReadProcessMemory(GetCurrentProcess(), LPVOID(addrInDumpPacket), buffer, packetSize, nullptr);
	return buffer;
}

inline LPBYTE allocateMem(LPCVOID _buffer, SIZE_T size) {
	auto allocatedMemory = LPBYTE(VirtualAllocEx(GetCurrentProcess(), nullptr, 0x100, MEM_COMMIT, PAGE_READWRITE));
	bool suces = WriteProcessMemory(GetCurrentProcess(), LPVOID(allocatedMemory), _buffer, size, nullptr);

	if (suces) {
		Console::writeLine("Sucess Allocating");
	}
	else {
		Console::writeLine("Error Allocating");
	}

	return allocatedMemory;
}
