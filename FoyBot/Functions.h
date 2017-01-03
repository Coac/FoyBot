#pragma once

#include <Psapi.h>


void PlaceJMP(BYTE *Address, DWORD jumpTo, DWORD length = 5)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;
	VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	dwRelAddr = (DWORD)(jumpTo - (DWORD)Address) - 5; //JMP size = 5
	*Address = 0xE9; //JMP opcode
	*((DWORD *)(Address + 0x1)) = dwRelAddr;
	for (DWORD x = 0x5; x < length; x++) { //Fill rest with NOP
		*(Address + x) = 0x90;
	}
	VirtualProtect(Address, length, dwOldProtect, &dwBkup);
}


MODULEINFO GetModuleInfo(char *szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}


DWORD FindPattern(char *module, char *pattern, char *mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}


		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}

