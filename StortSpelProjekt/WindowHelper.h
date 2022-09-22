#pragma once
#include "ErrorLog.h"
#include "MouseClass.h"

bool SetupWindow(HINSTANCE instance, UINT& width, UINT& height, int nCmdShow, HWND& window);
void SetUpMouse(MouseClass& mouseTo);