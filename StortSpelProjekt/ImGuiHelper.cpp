#include "ImGuiHelper.h"

ImGuiHelper::ImGuiHelper(Client*& client)
	:client(client)
{
	receivedMsg = "not yet received!";
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

		ImGui::Text("port: %d", client->getport());



		char buf[255]{};
		std::string ip;
		if (ImGui::InputText("ipAdress", buf, sizeof(buf)))
		{
			client->setIpAndPort(buf, 2001);
		}
		//if (ImGui::Button("start receive thread"))
		//{
		//	client->connectToServer();
		//	client->setupThread();
		//}

		char idBuf[255]{};// used to save identifier string for client
		if (ImGui::InputText("identifier", idBuf, sizeof(idBuf)))
		{
			client->saveText(idBuf);
		}

		char sendbuf[255]{};
		if (ImGui::InputText("save text to send to server", sendbuf, sizeof(sendbuf)))
		{
			//client->sendToServer(sendbuf);
			client->saveMsg(sendbuf);
		}

		/*if (ImGui::Button("send msg"))
		{
			client->sendToServerUdp();
		}*/

		if (ImGui::Button("connect TCP"))
		{
			client->connectToServer();
		}

		if (ImGui::Button("send message to server TCP"))
		{
			client->sendToServerTcp();
		}

		if (ImGui::Button("receive from server"))
		{
			client->receiveFromServerTcp();
		}
		if (ImGui::Button("Send to server packet 2"))
		{
			client->sendToServerTEMPTCP();
		}
		

		//selector.add(tcpSocket);

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
