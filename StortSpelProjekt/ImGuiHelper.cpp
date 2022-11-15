#include "stdafx.h"
#include "ImGuiHelper.h"
#include "PlanetGenerator.h"

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

		ImGui::Text("My port: %d", client->getport());



		char buf[255]{};
		std::string ip;
		if (ImGui::InputText("ipAdress", buf, sizeof(buf)))
		{
			client->setIpAndPort(buf, 2001);
		}

		//selector.add(tcpSocket);

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

void ImGuiHelper::react3D(bool& wireframe, bool &drawObjects, bool& landingMinigame, const float& dt, bool& velocityCamera)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();



	ImGui::NewFrame();
	{
		float fps = 1.f / dt;
		bool begun = ImGui::Begin("React Physics 3D");
		if (begun)
		{
			std::string tmpStr = "FPS: " + std::to_string(fps);
			ImGui::Text(tmpStr.c_str());
			ImGui::Checkbox("Draw collision boxes", &wireframe);
			ImGui::Checkbox("Draw objects", &drawObjects);
			ImGui::Checkbox("Landing Minigame", &landingMinigame);
			ImGui::Checkbox("Camera with velocity", &velocityCamera);
		}

		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHelper::planetEditor(PlanetImGuiInfo& planetImGuiStruct)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();



	ImGui::NewFrame();
	{
		bool begun = ImGui::Begin("Planet Editor");
		if (begun)
		{
			ImGui::SliderInt("Subdivisions", &planetImGuiStruct.currentSubdivisions, 0, 5);
			ImGui::SliderInt("Random Factor", &planetImGuiStruct.randomizedFactor, 0, 100);
			ImGui::Checkbox("Render Triangles", &planetImGuiStruct.renderTriangles);
			ImGui::Checkbox("Render Lines", &planetImGuiStruct.renderLines);

		}

		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
