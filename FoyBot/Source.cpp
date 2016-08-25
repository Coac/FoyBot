
#pragma comment(lib, "Ws2_32.lib")

#include "Console.h"
#include "Hooks.h"
#include"StringUtils.h"

void InitiateHooks()
{
	DWORD test = FindPattern("_FoY.exe", "\x55\x8B\xEC\x56\x8B\xF1\x80\x7E\x78\x00", "xxxxxxxxxx");
	sendFunctionAddr = test;
	//test += 80;

	TestJumpBack = test + 0x6;

	PlaceJMP((BYTE*)test, (DWORD)readPacketBeforeSendHook);

}

void processCmd(string &input) {
	vector<string> elems;
	split(input, ' ', elems);

	if (elems.size() < 2) {
		cout << "Syntax not correct";
	}
	else {
		string cmd = elems.front();
		if (cmd == "send") {		
			char buffer[10];
			
			int size = hex2bin(strdup(elems.at(1).c_str()), buffer);

			for (int i = 0; i < sizeof(buffer); i++)
			{
				cout << (char)(buffer[i] + 65);
			}

			// osnume red Novice potion
			//BYTE data[] = { 0x39, 0x04, 0x02, 0x00, 0xE9, 0x92, 0x1E, 0x00 }; 

			Send_To_Server((LPBYTE)buffer, size);
		}
	}

	cout << endl;
	
}

LPBYTE addressss = 0;
DWORD WINAPI sendServerThread()
{
	// on doit le reverse
	//39040200E9921E00

	for (;; Sleep(150))
	{
		string cmd;
		getline(cin, cmd);
		processCmd(cmd);


		if (GetAsyncKeyState(VK_UP))
		{
			
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

		InitiateHooks();

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)sendServerThread, NULL, NULL, NULL);
		break;

	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}