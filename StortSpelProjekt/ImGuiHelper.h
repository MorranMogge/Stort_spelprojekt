#pragma once

#include <iostream>
#include <DirectXMath.h>
#include "Client.h"

#include "imGUI\imconfig.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_dx11.h"
#include "imGUI\imgui_internal.h"
#include "imGUI\imstb_rectpack.h"
#include "imGUI\imstb_textedit.h"
#include "imGUI\imstb_truetype.h"
#include "imGUI\imgui_impl_win32.h"

class Player;

class ImGuiHelper
{
private:
	Client* client;
	std::string receivedMsg;
public:
	ImGuiHelper(Client*& client);
	ImGuiHelper();
	~ImGuiHelper();

	void drawInterface(std::string message);
	void setupImGui(float bgColour[]);
	void react3D(bool& wireframe, bool& drawObjects, DirectX::XMFLOAT3& wireframeClr, const float &dt, Player* currentPlayer);
	void spriteBegin();
	void spriteFloat(const std::string name, float& x, const float min, const float max);
	void spriteEnd();
	void react3D(bool& wireframe, bool& drawObjects, bool& landingMinigame, const float &dt, bool& velocityCamera, Player* currentPlayer);
};
