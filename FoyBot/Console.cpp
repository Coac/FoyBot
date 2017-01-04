#include "Console.hpp"


Console::Console()
{
}


Console::~Console()
{
}


void Console::create() {
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	int consoleHandleR, consoleHandleW;
	long stdioHandle;
	FILE *fptr;

	AllocConsole();
	SetConsoleTitle("FoyBot v0.1");

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);

	stdioHandle = long(GetStdHandle(STD_INPUT_HANDLE));
	consoleHandleR = _open_osfhandle(stdioHandle, _O_TEXT);
	fptr = _fdopen(consoleHandleR, "r");
	*stdin = *fptr;
	setvbuf(stdin, nullptr, _IONBF, 0);

	stdioHandle = long(GetStdHandle(STD_OUTPUT_HANDLE));
	consoleHandleW = _open_osfhandle(stdioHandle, _O_TEXT);
	fptr = _fdopen(consoleHandleW, "w");
	*stdout = *fptr;
	setvbuf(stdout, nullptr, _IONBF, 0);

	stdioHandle = long(GetStdHandle(STD_ERROR_HANDLE));
	*stderr = *fptr;
	setvbuf(stderr, nullptr, _IONBF, 0);

	freopen("conin$", "r+t", stdin);
	freopen("conout$", "w+t", stdout);
	freopen("conout$", "w+t", stderr);
}



void Console::setColor(const int &color) {
	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

// Need refactor duplicated code 
void Console::write(const char* fmt, ...) {
	if (!fmt)
		return;

	char buffer[4096];
	vsprintf(buffer, fmt, reinterpret_cast<char*>((&fmt) + 1));

	std::cout << buffer;

}
void Console::writeLine(const char* fmt, ...) {
	if (!fmt)
		return;

	char buffer[4096];
	vsprintf(buffer, fmt, reinterpret_cast<char*>((&fmt) + 1));

	std::cout << buffer << std::endl;
}

