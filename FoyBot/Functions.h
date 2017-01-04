#pragma once

#include <Psapi.h>


inline void PlaceJMP(BYTE *Address, DWORD jumpTo, DWORD length = 5)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;
	VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	dwRelAddr = DWORD(jumpTo - DWORD(Address)) - 5; //JMP size = 5
	*Address = 0xE9; //JMP opcode
	*reinterpret_cast<DWORD *>(Address + 0x1) = dwRelAddr;
	for (DWORD x = 0x5; x < length; x++) { //Fill rest with NOP
		*(Address + x) = 0x90;
	}
	VirtualProtect(Address, length, dwOldProtect, &dwBkup);
}


inline MODULEINFO GetModuleInfo(char *szModule)
{
	MODULEINFO modinfo = { nullptr };
	auto hModule = GetModuleHandle(szModule);
	if (hModule == nullptr)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}


inline DWORD FindPattern(char *module, char *pattern, char *mask)
{
	auto mInfo = GetModuleInfo(module);

	auto base = DWORD(mInfo.lpBaseOfDll);
	auto size = DWORD(mInfo.SizeOfImage);

	auto patternLength = DWORD(strlen(mask));

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		auto found = true;
		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *reinterpret_cast<char*>(base + i + j);
		}


		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}

