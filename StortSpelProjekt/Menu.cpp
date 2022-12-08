#include "stdafx.h"
#include "Menu.h"
#include "Input.h"
#include "Console.h"
#include "SoundCollection.h"
#include "GPU.h"
#include "SoundLibrary.h"

ID3D11ShaderResourceView* Menu::loadTexture(const std::string& fileName)
{
	manager.addTexture(fileName);
	return manager.getSrv(fileName);
}
Menu::Menu()
	:cam(Camera())
{
	SoundLibrary::menuMusic.setVolume(0.5f);
	SoundLibrary::menuMusic.play(true);
	meshes.push_back(new Mesh("../Meshes/Sphere"));
	meshes.push_back(new Mesh("../Meshes/reverseSphere"));
	meshes.push_back(new Mesh("../Meshes/astroid"));
	meshes.push_back(new Mesh("../Meshes/N1"));


	for (int i = 0; i < 5; i++)
	{
		if (i < 4) { planets.push_back(new Planet(meshes[2], DirectX::XMFLOAT3(i + 1.5f, i + 1.5f, i + 1.5f), DirectX::XMFLOAT3(-130 + i * 10, 5 - i * 0.5, -120 - i * 10), (4.0f * 9.82f))); }
		else
		{
			planets.push_back(new Planet(meshes[3], DirectX::XMFLOAT3(1.5f,1.5f, 1.5f), DirectX::XMFLOAT3(-130 + i * 10, 5 - i * 0.5, -120 - i * 10), (4.0f * 9.82f)));
		}
		planets[i]->setVelocity((4 - i*0.7f)*0.25f);
		planets[i]->setRotation(DirectX::SimpleMath::Vector3(1.2f * i, 0.2f * i, 0.7f * i));
		planets[i]->setRotationSpeed(DirectX::SimpleMath::Vector3(0.000f * i, 0.002f * (4-i), 0.000f * i));
	}
	planets.push_back(new Planet(meshes[0], DirectX::XMFLOAT3(40, 40, 40), DirectX::XMFLOAT3(-130, -30.f, 0.f), (4.0f * 9.82f), meshes[1]));
	
	planets.back()->setPlanetPosition({0 ,0,-0});
	//planets.back()->setPlanetScale({ 1, 1, 1 });
	cam.setPosition(DirectX::XMFLOAT3(0, 0, -140));
	cam.setCameraLookAt(DirectX::XMFLOAT3(90, 0, 0));
	DirectX::SimpleMath::Vector3 newTemp(-40, 0, -60);
	newTemp.Normalize();
	ltHandler.addLight(DirectX::XMFLOAT3(15, 10, -300), DirectX::XMFLOAT3(1, 1, 1), planets[2]->getPlanetPosition(), DirectX::XMFLOAT3(0, 1, 0), 1, 0.5f, 1000.5f);
	ltHandler.addLight(DirectX::XMFLOAT3(150, 10, -0), DirectX::XMFLOAT3(0,0,0.55f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2, 0.5f, 1000.5f);
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
	//Rotate main planet
	float constant = 0.00005f;
	DirectX::SimpleMath::Vector3 currentRot = planets.back()->getRotation();
	DirectX::SimpleMath::Vector3 targetRot(currentRot.x + constant, currentRot.y + constant, currentRot.z);
	planets.back()->setRotation(targetRot);
	planets[4]->setRotation(targetRot + targetRot);


	for (int i = 1; i < planets.size()-1; i++)
	{
		planets[i]->rotateAroundPoint(DirectX::XMFLOAT3(0, 0, 0));
		planets[i]->rotatePlanet();
	}

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
	//ltHandler.drawDebugMesh();

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