
#include <Windows.h>
#include <time.h>
#include "settings.h"

#ifndef _DEBUG
#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#endif

Settings g_Settings = { MAGIC, "127.0.0.1", 10086 };

extern "C" {void __stdcall StartKernel(CONST CHAR* szServerAddress, USHORT Port); }

int main()
{
	srand(time(NULL));
	StartKernel(g_Settings.RemoteAddr, g_Settings.RemotePort);
	return 0;
}
