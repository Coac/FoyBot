
#pragma comment(lib, "Ws2_32.lib")

#include "Console.h"
#include "Hooks.h"


void InitiateHooksTest()
{
	DWORD test = FindPattern("_FoY.exe", "\x55\x8B\xEC\x56\x8B\xF1\x80\x7E\x78\x00", "xxxxxxxxxx");
	sendFunctionAddr = test;
	//test += 80;

	TestJumpBack = test + 0x6;

	PlaceJMP((BYTE*)test, (DWORD)testHook);

}


LPBYTE addressss = 0;
DWORD WINAPI testThread()
{

	DWORD data[] = { 0x20439, 0x1e8e02 };
	addressss = allocateMem(&data, sizeof(data));
	Console::writeLine("%02x", (DWORD)addressss);
	for (;; Sleep(150))
	{

		if (GetAsyncKeyState(VK_UP))
		{
			Console::writeLine("Hey from key UP");
			Send_To_Server(addressss);
		}
	}
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Console::create();
		Console::writeLine("Success");

		InitiateHooksTest();

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)testThread, NULL, NULL, NULL);
		break;

	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}