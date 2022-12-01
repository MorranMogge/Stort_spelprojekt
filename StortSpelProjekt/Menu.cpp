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
	SoundLibrary::menuMusic.play(true);
	meshes.push_back(new Mesh("../Meshes/Sphere"));
	meshes.push_back(new Mesh("../Meshes/reverseSphere"));

	for (int i = 0; i < 5; i++)
	{
		planets.push_back(new Planet(meshes[0], DirectX::XMFLOAT3(i + 0.5f, i + 0.5f, i + 0.5f), DirectX::XMFLOAT3(0.f + i * 10.f, 0.f + i * 10.f, 0.f), (4.0f * 9.82f), meshes[1], 0));
		planets[i]->setVelocity((4 - i*0.7f)*0.25f);
		planets[i]->setRotation(DirectX::SimpleMath::Vector3(1.2f * i, 0.2f * i, 0.7f * i));
		planets[i]->setRotationSpeed(DirectX::SimpleMath::Vector3(0.000f * i, 0.002f * (4-i), 0.000f * i));

	}
	planets.push_back(new Planet(meshes[0], DirectX::XMFLOAT3(50, 50, 50), DirectX::XMFLOAT3(-130, -30.f, 0.f), (4.0f * 9.82f), meshes[1]));

	planets[4]->setPlanetPosition({ 20 ,0,-40 });
	planets[4]->setPlanetScale({ 1, 1, 1 });
	cam.setPosition(DirectX::XMFLOAT3(20, 0, -70));
	cam.setCameraLookAt(DirectX::XMFLOAT3(-20, 0, 10));
	DirectX::SimpleMath::Vector3 newTemp(-40, 0, -60);
	newTemp.Normalize();
	ltHandler.addLight(DirectX::XMFLOAT3(15, 10, 30), DirectX::XMFLOAT3(1, 1, 1), planets[4]->getPlanetPosition(), DirectX::XMFLOAT3(0, 1, 0), 1, 0.5f, 1000.5f);
	//ltHandler.addLight(DirectX::XMFLOAT3(15, 10, -0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(50, 50, 50), DirectX::XMFLOAT3(0, 1, 0), 1, 0.5f, 1000.5f);
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
	//for (int i = 1; i < planets.size(); i++)
	//{
	//	planets[i]->rotateAroundPoint(DirectX::XMFLOAT3(0, 0, 0));
	//	planets[i]->rotatePlanet();
	//}

	//Rotate main planet
	float constant = 0.00005f;
	DirectX::SimpleMath::Vector3 currentRot = planets[5]->getRotation();
	DirectX::SimpleMath::Vector3 targetRot(currentRot.x + constant, currentRot.y + constant, currentRot.z);
	planets[5]->setRotation(targetRot);


	return ui.GetGameState();
}

void Menu::drawShadows()
{
	int nrofLights = ltHandler.getNrOfLights();

	//Draw object shadow
	//for (int i = 0; i < nrofLights; i++)
	//{
	//	ltHandler.drawShadows(i, gameObjects);
	//}
	
	//Draw planet shadow
	for (int i = 0; i < nrofLights; i++)
	{
		ltHandler.drawShadows(i, planets);
	}
}

void Menu::drawFresnel()
{
	//Inverse
	basicRenderer.invFresnelPrePass();
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->drawAtmosphere();
	}
}

void Menu::drawObjects()
{
	//Bind light
	ltHandler.bindLightBuffers();

	//Draw Game objects
	//for (int i = 0; i < gameObjects.size(); i++)
	//{
	//	gameObjects[i]->draw();
	//}

	//Draw planets
	basicRenderer.tesselationPrePass(cam);
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->drawPlanet(true);
	}
	basicRenderer.resetTopology();

	//Draw with Ambient only shader
	basicRenderer.bindAmbientShader();

	//Draw light debug meshes
	ltHandler.drawDebugMesh();

	//Draw depth stencil
//basicRenderer.depthPrePass();
//ltHandler.drawShadows(0, gameObjects, &camera);
	GPU::immediateContext->OMSetDepthStencilState(nullptr, 0);
}

void Menu::Render()
{
	//Render shadow maps
	//basicRenderer.lightPrePass();
	//drawShadows();



	//Render Scene
	basicRenderer.setUpScene(this->cam, false);
	drawObjects();

	//Unbind light
	ltHandler.unbindSrv();

	//Render fresnel objects
	basicRenderer.fresnelPrePass(this->cam);
	this->drawFresnel();

	//Render Skybox
	basicRenderer.skyboxPrePass();
	skybox.draw();
	basicRenderer.depthUnbind();


	basicRenderer.postProcessPass();

	////Render Particles
	//basicRenderer.geometryPass(this->camera);
	//drawParticles();
	//basicRenderer.geometryUnbind();

	//Ui
	ui.Draw();
}