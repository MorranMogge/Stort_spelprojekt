#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "ServerTest.h"
#include <string>
#include <iostream>

#include "Game.h"
#include "Menu.h"
#include "WindowHelper.h"
#include "D3D11Helper.h"
#include "MemoryLeackChecker.h"

#include "ImGuiFunctions.h"

void serverTest()
{
	//sf::UdpSocket socket;
	//socket.setBlocking(false);
	////binds socket to a port
	//unsigned short port = 54000;
	//if (socket.bind(port) != sf::Socket::Done)
	//{
	//	ErrorLog::Log("Couldnt bind to a port\n");
	//}

	//sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	//char data[100] = "wiw";

	//if (socket.send(data, 100, ip, port) != sf::Socket::Done)
	//{
	//	// error...
	//}
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	std::string connectionType, mode;

	char buffer[2000];
	size_t recieved;
	std::string text = "Connect to: ";

	std::cout << "Enter (s) for Server, Enter (c) for client: " << std::endl;
	std::getline(std::cin, connectionType);
	//connectionType = "s";
	if (connectionType == "s")
	{
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		text += "Server";
		socket.send(text.c_str(), text.length() + 1);
	}
	else if (connectionType == "c")
	{
		socket.connect(ip, 2000);
		text += "Client";
		socket.send(text.c_str(), text.length() + 1);
	}

	socket.receive(buffer, sizeof(buffer), recieved);
	(void)getchar();
	std::cout << buffer << std::endl;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstace, _In_ LPWSTR lpCmdLine, _In_ int nCmdShhow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	serverTest();

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

	float clearColour[4]{ 0,0,0,0 };
	setupImGui(clearColour);




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
		
		immediateContext->ClearRenderTargetView(rtv, clearColour);
		immediateContext->OMSetRenderTargets(1, &rtv, dsView);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		immediateContext->RSSetViewports(1, &viewport);
		drawInterface();

		swapChain->Present(0, 0);
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