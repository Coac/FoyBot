#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>

#include <chrono>
#include <thread>


using namespace std;

// Thank you 'Zer0' for this function
bool InjectDynamicLibrary(DWORD processId, char* dllPath)
{
	// Open a new handle to the target process
	HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
	if (hTargetProcess) // if the handle is valid
	{
		// Kernel32.dll is always mapped to the same address in each process
		// So we can just copy the address of it & LoadLibraryA in OUR process and
		// expect it to be same in the remote process too.
		LPVOID LoadLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		// We must allocate more memory in the target process to hold the path for our dll in it's addresspace.
		LPVOID LoadPath = VirtualAllocEx(hTargetProcess, NULL, strlen(dllPath),
			MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(hTargetProcess, LoadPath, dllPath, strlen(dllPath), NULL);

		// Create a thread in the target process that will call LoadLibraryA() with the dllpath as a parameter
		HANDLE RemoteThread = CreateRemoteThread(hTargetProcess, 0, 0,
			(LPTHREAD_START_ROUTINE)LoadLibAddr, LoadPath, 0, 0);
		// Wait for the operation to complete, then continue.
		WaitForSingleObject(RemoteThread, INFINITE);

		// the path to the dll is no longer needed in the remote process, so we can just free the memory now.
		VirtualFreeEx(hTargetProcess, LoadPath, strlen(dllPath), MEM_RELEASE);
		CloseHandle(RemoteThread);
		CloseHandle(hTargetProcess);
		return true;
	}
	return false;
}



DWORD FindProcessId(wstring processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processSnapshot);
	return 0;
}

int main()
{
	int procId = FindProcessId(L"_FoY.exe");
	cout << "Process Id : " << procId << endl;

	bool hasInjected = InjectDynamicLibrary(procId, "C:\\Users\\Coac\\Documents\\PersonalProjects\\FoyBot\\Debug\\FoyBot.dll");
	if (hasInjected) {
		cout << "Success" << endl;
	}
	else {
		cout << "Failed" << endl;
		cout << "Try to run it as admin" << endl;
	}

	cout << "This console will be closed in 2 seconds." << endl;
	this_thread::sleep_for(chrono::milliseconds(2000));
}