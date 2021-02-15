#include <Windows.h>

int main()
{
	FreeConsole();
	MessageBeep(MB_ICONINFORMATION);
	MessageBoxA(null, "if you see this, it works", "Message", MB_OK);
}