#pragma once
#include <Windows.h>
#include <string>

class Console final
{
	inline static FILE* fpstdin = nullptr; //stdin
	inline static FILE* fpstdout = nullptr; //stdout
	inline static FILE* fpstderr = nullptr; //stderr
	inline static bool isActive = false;

public:
	static bool IsOpen()
	{
		return isActive;
	}
	static void Activate()
	{
		if (!isActive)
		{
			AllocConsole();

			freopen_s(&fpstdin, "CONIN$", "w", stdin); //freopen_s(&fpstdin, "CONIN$", "r", stdin);
			freopen_s(&fpstdout, "CONOUT$", "w", stdout);
			freopen_s(&fpstderr, "CONOUT$", "w", stderr);
			isActive = true;
		}
	}
	static void Destroy()
	{
		if (isActive)
		{
			//HWND myConsole = GetConsoleWindow(); //window handle
			fclose(stdin);
			fclose(stdout);
			fclose(stderr);
			FreeConsole();
			//ShowWindow(myConsole, 0);

			isActive = false;
		}

	}
};