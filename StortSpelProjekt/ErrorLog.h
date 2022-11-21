#pragma once
#define NOMINMAX
#include "stdafx.h"

class ErrorLog
{
public:
	static void Log(std::string message);
	static void Log(HRESULT hr, std::string message);
};