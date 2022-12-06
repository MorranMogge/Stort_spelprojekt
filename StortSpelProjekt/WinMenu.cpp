#include "stdafx.h"
#include "WinMenu.h"
#include "SoundLibrary.h"

GAMESTATE WinMenu::Update()
{
	float constant = 0.00005f;
	DirectX::SimpleMath::Vector3 currentRot = planets[5]->getRotation();
	DirectX::SimpleMath::Vector3 targetRot(currentRot.x + constant, currentRot.y + constant, currentRot.z);
	planets[5]->setRotation(targetRot);

    return ui.GetGameState();
}

void WinMenu::drawFresnel()
{
	//Inverse
	basicRenderer.invFresnelPrePass();
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->drawAtmosphere();
	}
}

void WinMenu::drawObjects()
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

void WinMenu::Render()
{

    basicRenderer.setUpScene();

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
	ui.Draw();
}

WinMenu::WinMenu()
{
	SoundLibrary::menuMusic.setVolume(0.5f);
	SoundLibrary::menuMusic.play(true);
	meshes.push_back(new Mesh("../Meshes/Sphere"));
	meshes.push_back(new Mesh("../Meshes/reverseSphere"));

	for (int i = 0; i < 5; i++)
	{
		planets.push_back(new Planet(meshes[0], DirectX::XMFLOAT3(i + 0.5f, i + 0.5f, i + 0.5f), DirectX::XMFLOAT3(0.f + i * 10.f, 0.f + i * 10.f, 0.f), (4.0f * 9.82f), meshes[1], 0));
		planets[i]->setVelocity((4 - i * 0.7f) * 0.25f);
		planets[i]->setRotation(DirectX::SimpleMath::Vector3(1.2f * i, 0.2f * i, 0.7f * i));
		planets[i]->setRotationSpeed(DirectX::SimpleMath::Vector3(0.000f * i, 0.002f * (4 - i), 0.000f * i));
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

	//manager.loadMeshAndBoneData("../Meshes/pinto_Run.fbx");
	manager.loadMeshAndBoneData("../Meshes/anim/character1_idle.fbx");
	manager.AdditionalAnimation("../Meshes/anim/character1_Dance1.fbx", "../Meshes/anim/character1_idle.fbx");
	manager.AdditionalAnimation("../Meshes/anim/character1_Dance2.fbx", "../Meshes/anim/character1_idle.fbx");
	manager.AdditionalAnimation("../Meshes/anim/character1_Dance3.fbx", "../Meshes/anim/character1_idle.fbx");
	manager.AdditionalAnimation("../Meshes/anim/character1_Sad1.fbx", "../Meshes/anim/character1_idle.fbx");
	manager.AdditionalAnimation("../Meshes/anim/character1_Sad2.fbx", "../Meshes/anim/character1_idle.fbx");
	manager.AdditionalAnimation("../Meshes/anim/character1_Sad3.fbx", "../Meshes/anim/character1_idle.fbx");

	manager.loadMeshAndBoneData("../Meshes/character2_idle.fbx");

	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

WinMenu::~WinMenu()
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