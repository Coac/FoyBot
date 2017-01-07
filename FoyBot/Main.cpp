
#pragma comment(lib, "Ws2_32.lib")

#include "Console.hpp"
#include "SendHook.hpp"
#include "RecvHook.hpp"
#include"StringUtils.h"
#include "Actions.hpp"

void InitiateHooks()
{
	auto addrSendFunc = FindPattern("_FoY.exe", "\x55\x8B\xEC\x56\x8B\xF1\x80\x7E\x78\x00", "xxxxxxxxxx");
	SendHook::sendFunctionAddr = addrSendFunc;
	Console::writeLine("sendFunctionAddr : %02X", SendHook::sendFunctionAddr);
	PlaceJMP(reinterpret_cast<BYTE*>(addrSendFunc), DWORD(SendHook::readPacketBeforeSendHook));
	SendHook::jumpBackSend = addrSendFunc + 0x6;

	auto addrRecvFunc = FindPattern("_FoY.exe", "\x3B\xC3\x7E\x22\x8D", "xxxxx");
	addrRecvFunc += 10;
	Console::writeLine("addrRecvFunc : %02X", addrRecvFunc);
	PlaceJMP(reinterpret_cast<BYTE*>(addrRecvFunc), DWORD(RecvHook::readPacketRecv));
	RecvHook::jumpBackRecv = addrRecvFunc + 0x5;
}


DWORD WINAPI simpleMoveAI()
{
	for (;; Sleep(1000))
	{
		auto mob = Store::getFirstMob();
		if (mob)
		{
			Actions:Actions::walkXY(mob->getPos()->getX(), mob->getPos()->getY());
			Sleep(500);
			Actions::attack(mob->getId());
		}
	}
}

void processCmd(string & input) {
	vector <string> elems;
	split(input, ' ', elems);

	auto cmd = elems.front();

	if (cmd == "send" && elems.size() == 2) {

		char buffer[100];

		auto size = hex2bin(strdup(elems.at(1).c_str()), buffer);

		for (auto i = 0; i < sizeof(buffer); i++) {
			cout << char(buffer[i] + 65);
		}

		// osnume red Novice potion
		//BYTE data[] = { 0x39, 0x04, 0x02, 0x00, 0xE9, 0x92, 0x1E, 0x00 }; 

		SendHook::sendPacket(LPBYTE(buffer), size);
	}
	else if (cmd == "walk" && elems.size() == 3) {
		Actions::walkXY(stoi(elems.at(1)), stoi(elems.at(2)));
	}
	else if (cmd == "startAI") {
		CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(simpleMoveAI), nullptr, NULL, nullptr);
	}
	else {
		cout << "Syntax not correct";
	}

	cout << endl;

}

LPBYTE addressss = nullptr;
DWORD WINAPI sendServerThread()
{
	// on doit le reverse
	//39040200E9921E00

	for (;; Sleep(150))
	{
		string cmd;
		getline(cin, cmd);
		processCmd(cmd);
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

		CreateThread(nullptr, NULL, LPTHREAD_START_ROUTINE(sendServerThread), nullptr, NULL, nullptr);
		break;

	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	default: ;
	}
	return TRUE;
}