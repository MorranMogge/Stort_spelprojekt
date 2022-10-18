#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <time.h>

#include "Console.h"
#include "MemoryLeackChecker.h"
#include "SoundCollection.h"
#include "Client.h"
#include "Game.h"
#include "Menu.h"
#include "WindowHelper.h"
#include "D3D11Helper.h"

#include "GuiHandler.h"
#include "ImGuiHelper.h"

#include "SettingsMenu.h"
#include "CreditsMenu.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstace, _In_ LPWSTR lpCmdLine, _In_ int nCmdShhow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand((unsigned)time(0));

	SoundCollection::Load();

	Console::Activate(); // activate console for cout and cin, to destroy console call "Console::Destroy();" 
	std::cout << "test print \n"; //test print

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().IniFilename = nullptr;

	UINT WIDTH = 1280;
	UINT HEIGHT = 720;
	HWND window;

	Client* client = new Client();

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShhow, window))
	{
		ErrorLog::Log("Could not set up window!");
		return -1;
	}

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	if (!CreateInterfaces(WIDTH, HEIGHT, window, device, immediateContext, swapChain))
	{
		ErrorLog::Log("Could not set up D3D11!");
		return -1;
	}

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, immediateContext);

	//State* currentState = new Game(immediateContext, device, swapChain, mouse, window);
	State* currentState = new Menu();
	GAMESTATE stateInfo = NOCHANGE;

	MSG msg = {};
	

	while (msg.message != WM_QUIT && stateInfo != EXIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sound::Update();

		


		stateInfo = currentState->Update();

		if (GetAsyncKeyState(VK_ESCAPE))
			stateInfo = EXIT;

		if (stateInfo != NOCHANGE)
		{
			switch (stateInfo)
			{
			case GAME:
				delete currentState;
				currentState = new Game(immediateContext, device, swapChain, window);
				break;
			case SETTINGS:
				delete currentState;
				currentState = new SettingsMenu();
				break;
			case CREDITS:
				delete currentState;
				currentState = new CreditsMenu();
				break;
			case MENU:
				delete currentState;
				currentState = new Menu();
				break;
			default:
				break;
			}
		}
		
		currentState->Render();

		//immediateContext->ClearRenderTargetView(rtv, clearColour);
		/*immediateContext->OMSetRenderTargets(1, &rtv, dsView);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		immediateContext->RSSetViewports(1, &viewport);

		currentState->DrawUI();
		*/
		imGuiHelper.drawInterface("test");
		swapChain->Present(0, 0);
	}

	#pragma region Deallocation
	delete currentState;
	
	delete client;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (Console::IsOpen())
		Console::Destroy();

	device->Release();
	immediateContext->Release();
	swapChain->Release();

	#pragma endregion
	
	return 0;
}