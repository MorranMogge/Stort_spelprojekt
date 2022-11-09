#pragma once
#include "ErrorLog.h"

bool SetupWindow(HINSTANCE instance, UINT& width, UINT& height, int nCmdShow, HWND& window);
void ChangeResolution(const UINT width, const UINT height);