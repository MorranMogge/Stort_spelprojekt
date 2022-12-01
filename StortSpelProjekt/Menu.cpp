#include "stdafx.h"
#include "Menu.h"
#include "Input.h"
#include "Console.h"
#include "SoundCollection.h"
#include "GPU.h"
#include "SoundLibrary.h"

Menu::Menu()
	:cam(Camera())
{
	SoundLibrary::menuMusic.setVolume(0.5f);
	//SoundLibrary::menuMusic.play(true);

	meshes.push_back(new Mesh("../Meshes/Sphere"));
	for (int i = 0; i < 4; i++)
	{
		planets.push_back(new Planet(meshes.back(), DirectX::XMFLOAT3(i + 0.5f, i + 0.5f, i + 0.5f), DirectX::XMFLOAT3(0.f + i * 10.f, 0.f + i * 10.f, 0.f)));
		planets[i]->setVelocity((4 - i*0.7f)*0.25f);
		planets[i]->setRotation(DirectX::SimpleMath::Vector3(1.2f * i, 0.2f * i, 0.7f * i));
		planets[i]->setRotationSpeed(DirectX::SimpleMath::Vector3(0.000f * i, 0.002f * (4-i), 0.000f * i));
	}
	cam.setPosition(DirectX::XMFLOAT3(20, 0, -70));
	cam.setCameraLookAt(DirectX::XMFLOAT3(-20, 0, 10));
	DirectX::SimpleMath::Vector3 newTemp(-40, 0, -60);
	newTemp.Normalize();
	ltHandler.addLight(DirectX::XMFLOAT3(20, 0, -70), DirectX::XMFLOAT3(1, 1, 1), newTemp, DirectX::XMFLOAT3(0, 1, 0), 1);

	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

Menu::~Menu()
{
	for (int i = 0; i < planets.size(); i++)
	{
		delete planets[i];
	}
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
}

GAMESTATE Menu::Update()
{
	for (int i = 1; i < planets.size(); i++)
	{
		planets[i]->rotateAroundPoint(DirectX::XMFLOAT3(0, 0, 0));
		planets[i]->rotatePlanet();
	}
	return ui.GetGameState();
}

void Menu::Render()
{
	basicRenderer.lightPrePass();
	for (int i = 0; i < ltHandler.getNrOfLights(); i++)
	{
		ltHandler.drawShadows(i, planets, &cam);
	}

	GPU::immediateContext->OMSetDepthStencilState(nullptr, 0);
	//Bind light
	ltHandler.bindLightBuffers();

	basicRenderer.setUpScene();
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->drawPlanet();
	}
	//Render Skybox
	basicRenderer.skyboxPrePass();
	skybox.draw();
	basicRenderer.depthUnbind();
	ui.Draw();
}
