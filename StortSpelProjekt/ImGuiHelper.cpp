#include "stdafx.h"
#include "ImGuiHelper.h"
#include "Profiler.h"
#include "Player.h"
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

void ImGuiHelper::react3D(bool& wireframe, bool &drawObjects, bool& landingMinigame, const float& dt, bool& velocityCamera, Player* currentPlayer)
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

			ImVec4 col(0.0f, 0.8f, 0.0f, 1.0f);
			ImGui::TextColored(col, ("Vram: " + std::to_string(getVramUsage())).c_str());
			ImGui::TextColored(col, ("Ram: " + std::to_string(getRamUsage())).c_str());


			ImGui::Checkbox("Draw collision boxes", &wireframe);
			ImGui::Checkbox("Draw objects", &drawObjects);

			if (ImGui::Button("vram Usage"))
			{
				std::cout << std::to_string(getVramUsage());
			}
			std::string tmpStrX = "xPos: "+std::to_string(currentPlayer->getPos().x);
			std::string tmpStrY = "yPos: " + std::to_string(currentPlayer->getPos().y);
			std::string tmpStrZ = "zPos: " + std::to_string(currentPlayer->getPos().z);
			ImGui::Checkbox("Landing Minigame", &landingMinigame);
			ImGui::Checkbox("Camera with collision", &velocityCamera);
			ImGui::Text(tmpStrX.c_str());
			ImGui::Text(tmpStrY.c_str());
			ImGui::Text(tmpStrZ.c_str());

			std::string playerHoldingItem;
			if (currentPlayer->getItem() == nullptr)
			{
				playerHoldingItem = "Player item == nullptr\n";
			}
			else
			{
				playerHoldingItem = "Player item != nullptr\n";
			}

			ImGui::Text(playerHoldingItem.c_str());
		}

		ImGui::End();
	}
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}



void ImGuiHelper::spriteBegin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiHelper::spriteFloat(const std::string name,float& x, const float min, const float max)
{
	ImGui::Begin("sprite pos: ");

	ImGui::SliderFloat(std::string(name + " X: ").c_str(), &x, min, max);

	ImGui::End();
}

void ImGuiHelper::spriteEnd()
{
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiHelper::planetEditor(PlanetImGuiInfo& planetImGuiStruct)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	float colours[5][3];
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));

	for (int i = 0; i < 5; i++)
	{
		colours[i][0] = planetImGuiStruct.colourSelection[i].x;
		colours[i][1] = planetImGuiStruct.colourSelection[i].y;
		colours[i][2] = planetImGuiStruct.colourSelection[i].z;
	}

	ImGui::NewFrame();
	{
		bool begun = ImGui::Begin("Planet Editor");
		if (begun)
		{
			ImGui::TextColored(ImVec4(0, 1, 0, 1), "Rendering");
			ImGui::Checkbox("Render Triangles", &planetImGuiStruct.renderTriangles);
			ImGui::Checkbox("Render Lines", &planetImGuiStruct.renderLines);
			ImGui::Text("");
			ImGui::TextColored(ImVec4(1, 0, 1, 1), "Mesh Creator");
			if (ImGui::Button("Recreate Mesh with selected settings")) planetImGuiStruct.recreateMesh = true;
			ImGui::SliderInt("Elevation Factor", &planetImGuiStruct.randomizedFactor, 0, 500);
			ImGui::SliderInt("Subdivisions", &planetImGuiStruct.currentSubdivisions, 1, 6);
			ImGui::Checkbox("Normalized", &planetImGuiStruct.normalised);



			if (ImGui::Button("Recreate Original Sphere")) planetImGuiStruct.recreateOriginalSphere = true;




			//if (ImGui::Button("Update Colours")) planetImGuiStruct.updateColours = true;

			ImGui::Text("");
			ImGui::TextColored(ImVec4(1, 165.f / 256.f, 0, 1), "Save mesh to file");

			if (ImGui::InputText("File Name", buffer, sizeof(buffer))) planetImGuiStruct.fileName = buffer;
			if (ImGui::Button("Save as Obj")) planetImGuiStruct.saveObj = true;

		}
		ImGui::End();


		bool yo = ImGui::Begin("Colour Editor");
		if (yo)
		{
			ImGui::TextColored(ImVec4(255.f / 255.f, 115 / 255.f, 232 / 255.f, 1), "Terrain Colour");

			ImGui::ColorEdit3("Ground", colours[0]);
			ImGui::ColorEdit3("Sea", colours[1]);
			ImGui::ColorEdit3("Hill", colours[2]);
			ImGui::ColorEdit3("Deep Sea", colours[3]);
			ImGui::ColorEdit3("Mountain", colours[4]);
			ImGui::Text("");
			ImGui::TextColored(ImVec4(93 / 255.f, 0 / 255.f, 255 / 255.f, 1), "Colour height factor");
			ImGui::SliderFloat("Ground", &planetImGuiStruct.colourFactor[0], 0.5, 1.5);
			ImGui::SliderFloat("Sea", &planetImGuiStruct.colourFactor[1], 0.5, 1.5);
			ImGui::SliderFloat("Hill", &planetImGuiStruct.colourFactor[2], 0.5, 1.5);
			ImGui::SliderFloat("Deep Sea", &planetImGuiStruct.colourFactor[3], 0.5, 1.5);
			ImGui::SliderFloat("Mountain", &planetImGuiStruct.colourFactor[4], 0.5, 1.5);
			std::string l;
			l = "Min length: " + std::to_string(planetImGuiStruct.minLength);
			ImGui::Text(l.c_str());
			l = "Max length: " + std::to_string(planetImGuiStruct.maxLength);
			ImGui::Text(l.c_str());

			if (ImGui::Button("Base Factors of vertice height"))
			{
				planetImGuiStruct.colourFactor[3] = planetImGuiStruct.minLength + 0.05f;
				planetImGuiStruct.colourFactor[4] = planetImGuiStruct.maxLength - 0.05f;;
				planetImGuiStruct.colourFactor[0] = (planetImGuiStruct.minLength + planetImGuiStruct.maxLength) / 2.f;
				planetImGuiStruct.colourFactor[2] = (planetImGuiStruct.colourFactor[4] + planetImGuiStruct.colourFactor[0]) / 2.f;
				planetImGuiStruct.colourFactor[1] = (planetImGuiStruct.colourFactor[3] + planetImGuiStruct.colourFactor[0]) / 2.f;

			}

			if (ImGui::Button("Reset Factors"))
			{
				planetImGuiStruct.colourFactor[0] = 1.0f;
				planetImGuiStruct.colourFactor[1] = 0.95f;
				planetImGuiStruct.colourFactor[2] = 1.1f;
				planetImGuiStruct.colourFactor[3] = 0.85f;
				planetImGuiStruct.colourFactor[4] = 1.2f;
			}
			ImGui::Checkbox("Update Colours", &planetImGuiStruct.updateColours);
			ImGui::SliderInt("Colours used", &planetImGuiStruct.useColours, 0, 5);
			if (ImGui::Button("Randomize Colours"))
			{
				for (int i = 0; i < 5; i++)
				{
					colours[i][0] = (rand() % 256) / 256.f;
					colours[i][1] = (rand() % 256) / 256.f;
					colours[i][2] = (rand() % 256) / 256.f;
				}
			}
		}
		//ImGui::EndPopup();
		ImGui::End();

	}

	for (int i = 0; i < 5; i++)
	{
		planetImGuiStruct.colourSelection[i].x = colours[i][0];
		planetImGuiStruct.colourSelection[i].y = colours[i][1];
		planetImGuiStruct.colourSelection[i].z = colours[i][2];

	}
	if (planetImGuiStruct.useColours > 1 && planetImGuiStruct.colourFactor[1] > planetImGuiStruct.colourFactor[0]) planetImGuiStruct.colourFactor[1] = planetImGuiStruct.colourFactor[0];
	if (planetImGuiStruct.useColours > 2 && planetImGuiStruct.colourFactor[2] < planetImGuiStruct.colourFactor[0]) planetImGuiStruct.colourFactor[2] = planetImGuiStruct.colourFactor[0];
	if (planetImGuiStruct.useColours > 3 && planetImGuiStruct.colourFactor[3] > planetImGuiStruct.colourFactor[1]) planetImGuiStruct.colourFactor[3] = planetImGuiStruct.colourFactor[1];
	if (planetImGuiStruct.useColours > 4 && planetImGuiStruct.colourFactor[4] < planetImGuiStruct.colourFactor[2]) planetImGuiStruct.colourFactor[4] = planetImGuiStruct.colourFactor[2];


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
