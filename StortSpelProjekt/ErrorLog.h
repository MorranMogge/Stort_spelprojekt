#pragma once
#include <Windows.h>
#include <string>

class ErrorLog
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
};