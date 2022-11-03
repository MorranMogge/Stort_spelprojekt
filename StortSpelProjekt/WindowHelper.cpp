#include "stdafx.h"
#include "WindowHelper.h"
#include "GPU.h"
#include "resource.h"
#include "imGUI\imconfig.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_dx11.h"
#include "imGUI\imgui_internal.h"
#include "imGUI\imstb_rectpack.h"
#include "imGUI\imstb_textedit.h"
#include "imGUI\imstb_truetype.h"
#include "imGUI\imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return true;
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SetupWindow(HINSTANCE instance, UINT& width, UINT& height, int nCmdShow, HWND& window)
{
	const wchar_t CLASS_NAME[] = L"Window Class";

	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(wc); //uint size of structer
	wc.style = CS_OWNDC; //uint
	wc.cbClsExtra = 0; //0, not need extra data
	wc.cbWndExtra = 0; //0, not need extra bytes
	wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = CLASS_NAME;

	wc.hIcon = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hIconSm = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, LONG(width), LONG(height) };

	window = CreateWindowEx(
		0, //style, 0, not need style
		CLASS_NAME, //class name
		L"Projekt", //window name
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//window style,F1 for more details, type of DWORD
		//WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, //window style,F1 for more details, type of DWORD
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, //window start position on screen
		wr.right - wr.left, wr.bottom - wr.top, //window size
		nullptr,  //parent, null means no parent
		nullptr, //handle menu,null means no handle
		instance, //hInstance of window
		nullptr);//custom parameter

	if (window == nullptr)
	{
		std::cerr << "HWND was, last error: " << GetLastError() << std::endl;
		return false;
	}

	WINDOWINFO info{};
	if (GetWindowInfo(window, &info))
	{
		RECT rect;
		rect = info.rcClient;
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		GPU::windowWidth = width;
		GPU::windowHeight = height;
	}

	ShowWindow(window, nCmdShow);
	return true;
}