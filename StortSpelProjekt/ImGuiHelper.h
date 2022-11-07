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
	void react3D(bool& wireframe, bool& drawObjects, DirectX::XMFLOAT3& wireframeClr, const float &dt);

	void spriteBegin();
	void spriteFloat(std::string name, float& x, float min, float max);
	void spriteEnd();
};
