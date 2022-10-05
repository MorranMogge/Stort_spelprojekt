#include "ImGuiHelper.h"

ImGuiHelper::ImGuiHelper(Client*& client)
	:client(client)
{
	receivedMsg = "not yet received!";
}

ImGuiHelper::ImGuiHelper()
	:client(nullptr)
{
}

ImGuiHelper::~ImGuiHelper()
{
}

void ImGuiHelper::drawInterface(std::string message)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	

	ImGui::NewFrame();
	{
		bool begun = ImGui::Begin("Stranded engineers");
		if (begun)
		{
			
		}
		char buf[255]{};
		std::string ip;
		if (ImGui::InputText("ipAdress", buf, sizeof(buf)))
		{
			client->setIpAndPort(buf, 2001);
		}
		if (ImGui::Button("Connect to server"))
		{
			client->connectToServer();
			client->setupThread();
		}

		ImGui::Text(client->receive().c_str());

		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHelper::setupImGui(float bgColour[])
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.5f, 0.0f, 1.00f);
}

void ImGuiHelper::react3D(bool& wireframe, bool &drawObjects, DirectX::XMFLOAT3& wireframeClr, const float& dt)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();



	ImGui::NewFrame();
	{
		float fps = 1.f / dt;
		float wClr[3]{ wireframeClr.x, wireframeClr.y, wireframeClr.z };
		bool begun = ImGui::Begin("React Physics 3D");
		if (begun)
		{
			std::string tmpStr = "FPS: " + std::to_string(fps);
			ImGui::Text(tmpStr.c_str());
			ImGui::Checkbox("Draw collision boxes", &wireframe);
			ImGui::Checkbox("Draw objects", &drawObjects);
			ImGui::ColorEdit3("Wireframe colour", wClr);
		}
		wireframeClr.x = wClr[0];
		wireframeClr.y = wClr[1];
		wireframeClr.z = wClr[2];

		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
