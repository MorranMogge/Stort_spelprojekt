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

struct PlanetImGuiInfo;

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
	void react3D(bool& wireframe, bool& drawObjects, bool& landingMinigame, const float &dt, bool& velocityCamera);
	void planetEditor(PlanetImGuiInfo& planetImGuiStruct);
};
