#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{
	float planetSize = 50.f;
	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(planetSize, planetSize, planetSize));
	player = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(22*3, 12*3, -22*3), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	otherPlayer = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(-22, 12, 22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	potion = new Potion("../Meshes/Baseball", DirectX::SimpleMath::Vector3(10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	testBat = new BaseballBat("../Meshes/Baseball", DirectX::SimpleMath::Vector3(-10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	testCube = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	maxDistance = planet->getScale().x + 2.f;

	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(testBat, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(otherPlayer, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::SPHERE, planet->getScale());
	planet->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);

	testCube->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	testBat->getPhysComp()->setPosition(reactphysics3d::Vector3(testBat->getPosV3().x, testBat->getPosV3().y, testBat->getPosV3().z));
	otherPlayer->getPhysComp()->setPosition(reactphysics3d::Vector3(otherPlayer->getPosV3().x, otherPlayer->getPosV3().y, otherPlayer->getPosV3().z));

	gameObjects.emplace_back(planet);
	gameObjects.emplace_back(player);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testCube);
	gameObjects.emplace_back(testBat);
	gameObjects.emplace_back(otherPlayer);


	for (int i = 0; i < 10; i++)
	{
		GameObject* newObj = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		physWolrd.addPhysComponent(newObj, reactphysics3d::CollisionShapeName::BOX);
		newObj->getPhysComp()->setPosition(reactphysics3d::Vector3(-100, 120+(float)i*10, 100));
		gameObjects.emplace_back(newObj);
	}

	testBat->setPlayer(player);
	testBat->setTestObj(gameObjects);
	player->setPhysComp(physWolrd.getPlayerBox());
}

void Game::drawShadows()
{
	for (int i = 0; i < ltHandler.getNrOfLights(); i++)
	{
		ltHandler.drawShadows(i, gameObjects);
	}

	basicRenderer.depthPrePass();
	ltHandler.drawShadows(0, gameObjects, &camera);
	GPU::immediateContext->OMSetDepthStencilState(nullptr, 0);
}

void Game::drawObjects(bool drawDebug)
{
	//Bind light
	ltHandler.bindLightBuffers();

	//Draw Game objects
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects.at(i)->draw();
	}
	//Draw light debug meshes
	if (drawDebug)
	{
		basicRenderer.bindAmbientShader();
		ltHandler.drawDebugMesh();
	}

	//Unbind light
	ltHandler.unbindSrv();
}

void Game::drawParticles()
{
	for (int i = 0; i < this->ptEmitters.size(); i++)
	{
		if (!this->ptEmitters.at(i).isActive() && this->ptEmitters.at(i).isPassComplete())
		{
			//turn off
		}
		else
		{
			this->ptEmitters.at(i).BindAndDraw();
		}
	}
}

bool Game::setUpWireframe()
{
	reactWireframeInfo.wireframeClr = DirectX::XMFLOAT3(1.f, 0.f, 0.f);

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(wirefameInfo);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &reactWireframeInfo;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &wireBuffer);
	return !FAILED(hr);
}

void Game::updateBuffers()
{
	//Update GameObjects

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->updateBuffer();
	}

	//Update Wireframe buffer
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(wireBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &reactWireframeInfo, sizeof(wirefameInfo));
	immediateContext->Unmap(wireBuffer, 0);
}

void Game::handleKeybinds()
{
	if (GetAsyncKeyState('C')) physWolrd.addBoxToWorld();

	if (Input::KeyPress(KeyCode::T))
	{
		DirectX::XMFLOAT3 test(this->ptEmitters.at(0).getPosition().x, this->ptEmitters.at(0).getPosition().y, this->ptEmitters.at(0).getPosition().z - 10);
		this->ptEmitters.at(0).setPosition(test);
		this->ptEmitters.at(0).setRotation(DirectX::XMFLOAT3(-1, 0, 0));
		this->ptEmitters.at(0).updateBuffer();
	}
	if (Input::KeyPress(KeyCode::Y))
	{
		this->ptEmitters.at(0).setActive(false);
		this->ptEmitters.at(0).updateBuffer();
	}
	if (Input::KeyPress(KeyCode::U))
	{
		this->ptEmitters.at(0).setActive(true);
		this->ptEmitters.at(0).updateBuffer();
	}
	if (Input::KeyPress(KeyCode::I))
	{
		drawDebug = true;
	}
	if (Input::KeyPress(KeyCode::O))
	{
		drawDebug = false;
	}
}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpWireframe();
	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(20, 30, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(10, -20, 30), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2,5)));

	currentTime = std::chrono::system_clock::now();
	lastUpdate = currentTime;
	playerVecRenderer.setPlayer(player);
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();
}

Game::~Game()
{

	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		if (this->gameObjects.at(i) != nullptr)
		{
			delete this->gameObjects.at(i);
		}
	}

	wireBuffer->Release();
}

GAMESTATE Game::Update()
{
	//Get newest delta time
	lastUpdate = currentTime;
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();

	
	grav = planetGravityField.calcGravFactor(player->getPosV3());
	player->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, dt);
	
	scalarMultiplicationXMFLOAT3(dt, grav);
	additionXMFLOAT3(velocity, grav);
	
	//reactphysics3d::Ray ray(reactphysics3d::Vector3(this->player->getPosV3().x, this->player->getPosV3().y, this->player->getPosV3().z), reactphysics3d::Vector3(player->getRayCastPos()));
	reactphysics3d::Ray ray(reactphysics3d::Vector3(player->getRayCastPos()), reactphysics3d::Vector3(0,0,0));
	reactphysics3d::RaycastInfo rayInfo;
	DirectX::XMFLOAT3 tempp(1,1,1);
	int gameObjSize = gameObjects.size();
	for (int i = 1; i < gameObjSize; i++)
	{
		if (gameObjects[i]->getPhysComp()->raycast(ray, rayInfo)) { velocity = DirectX::XMFLOAT3(0, 0, 0); std::cout << "GameObj ID:" << i << "\n"; tempp = DirectX::XMFLOAT3(rayInfo.worldPoint.x, rayInfo.worldPoint.y, rayInfo.worldPoint.z); break; }
	}

	//if (tempp.x != 1.f) std::cout << "RayCast length: " << getLength(tempp) << "\n";
	subtractionXMFLOAT3(tempp, this->player->getPos());

	//Keeps player at the surface of the planet
	if (getLength(tempp) <= 0.1f) { velocity = DirectX::XMFLOAT3(0, 0, 0); }
	player->movePos(velocity);

	//Player functions
	player->pickupItem(potion);
	player->pickupItem(testBat);
	
	physWolrd.updatePlayerBox(player->getPos());
	physWolrd.update(dt);

	camera.moveCamera(player->getPosV3(), player->getRotationMX(), dt);
	for (int i = 1; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
	//Here you can write client-server related functions?


	this->updateBuffers();
	
	if (player->repairedShip()) { std::cout << "You have repaired the ship and returned to earth\n"; return EXIT; }
	
	//Debug keybinds
	this->handleKeybinds();
	return NOCHANGE;
}

void Game::Render()
{
	//Render shadow maps
	basicRenderer.lightPrePass();
	drawShadows();
	
	//Render Scene
	basicRenderer.setUpScene(this->camera);
	if (objectDraw) drawObjects(drawDebug);

	//Render Skybox
	basicRenderer.skyboxPrePass();
	this->skybox.draw();
	basicRenderer.depthUnbind();

	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (wireframe) { immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); playerVecRenderer.drawLines(); }

	//Render Particles
	basicRenderer.geometryPass(this->camera);
	drawParticles();
	this->ptEmitters.at(0).unbind();
}

