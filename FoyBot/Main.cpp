
#pragma comment(lib, "Ws2_32.lib")

#include "Console.hpp"
#include "Hooks.h"
#include "SendHook.hpp"
#include "RecvHook.hpp"
#include"StringUtils.h"

void InitiateHooks()
{
	DWORD addrSendFunc = FindPattern("_FoY.exe", "\x55\x8B\xEC\x56\x8B\xF1\x80\x7E\x78\x00", "xxxxxxxxxx");
	sendFunctionAddr = addrSendFunc;
	Console::writeLine("sendFunctionAddr : %02X", sendFunctionAddr);
	PlaceJMP((BYTE*)addrSendFunc, (DWORD)SendHook::readPacketBeforeSendHook);
	SendHook::jumpBackSend = addrSendFunc + 0x6;


	DWORD addrRecvFunc = FindPattern("_FoY.exe", "\x3B\xC3\x7E\x22\x8D", "xxxxx");
	addrRecvFunc += 10;
	Console::writeLine("addrRecvFunc : %02X", addrRecvFunc);
	PlaceJMP((BYTE*)addrRecvFunc, (DWORD)RecvHook::readPacketRecv);
	RecvHook::jumpBackRecv = addrRecvFunc + 0x5;

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
			char buffer[100];
			
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
		Console::setColor(12);
		Console::writeLine("Succesfully injected");
		Console::setColor(11);
		Console::writeLine("Started FoyBoy v0.1");

		Console::setColor(7);

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