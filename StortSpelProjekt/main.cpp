
#include "Game.h"
#include "Menu.h"
#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "MemoryLeackChecker.h"

#include "imGUI\imconfig.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_dx11.h"
#include "imGUI\imgui_internal.h"
#include "imGUI\imstb_rectpack.h"
#include "imGUI\imstb_textedit.h"
#include "imGUI\imstb_truetype.h"
#include "imGUI\imgui_impl_win32.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstace, _In_ LPWSTR lpCmdLine, _In_ int nCmdShhow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	UINT WIDTH = 1080;
	UINT HEIGHT = 720;
	HWND window;

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShhow, window))
	{
		ErrorLog::Log("Could not set up window!");
		return -1;
	}

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsView;
	ID3D11Texture2D* dsTexture;

	D3D11_VIEWPORT viewport;

	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		ErrorLog::Log("Could not set up D3D11!");
		return -1;
	}

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, immediateContext);

	State* currentState = DBG_NEW Menu();
	GAMESTATE stateInfo = NOCHANGE;

	MSG msg = {};

	while (msg.message != WM_QUIT && stateInfo != EXIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		stateInfo = currentState->Update();

		if (stateInfo != NOCHANGE)
		{
			switch (stateInfo)
			{
			case MENU:
				delete currentState;
				currentState = new Menu();
				break;
			case GAME:
				delete currentState;
				currentState = new Game();
				break;
			default:
				break;
			}
		}
	
		currentState->Render();
	}

	#pragma region Deallocation
	delete currentState;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	device->Release();
	immediateContext->Release();
	swapChain->Release();
	rtv->Release();
	dsView->Release();
	dsTexture->Release();
	#pragma endregion
	
	return 0;
}