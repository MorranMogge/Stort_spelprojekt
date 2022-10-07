#include "stdafx.h"
#include "ErrorLog.h"
#include <comdef.h>

void ErrorLog::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLog::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring wide_string(message.begin(), message.end());
	std::wstring error_message = L"Error: " + wide_string + L"\n" + error.ErrorMessage();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}