#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{
	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(20.0f, 20.0f, 20.0f));
	player = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	potion = new Potion("../Meshes/Baseball", DirectX::SimpleMath::Vector3(10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	testCube = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	DirectX::SimpleMath::Vector3 downVector = planetGravityField.calcGravFactor(DirectX::XMFLOAT3(10, 20, 10));
	
	////downVector *= -1;

	//DirectX::SimpleMath::Vector4 column1;
	//DirectX::SimpleMath::Vector4 column2;
	//DirectX::SimpleMath::Vector4 column3;
	//DirectX::SimpleMath::Vector4 column4;

	//DirectX::SimpleMath::Vector3 up(0, 1, 0);
	//DirectX::SimpleMath::Vector3 xaxis = downVector;
	//xaxis.Cross(up, xaxis);
	//xaxis.Normalize();

	//DirectX::SimpleMath::Vector3 yaxis = xaxis;
	//yaxis.Cross(downVector, yaxis);
	//yaxis.Normalize();


	//column1.x = xaxis.x;
	//column1.y = yaxis.x;
	//column1.z = downVector.x;
	//column1.w = 0;

	//column2.x = xaxis.y;
	//column2.y = yaxis.y;
	//column2.z = downVector.y;
	//column1.w = 0;

	//column3.x = xaxis.z;
	//column3.y = yaxis.z;
	//column3.z = downVector.z;
	//column1.w = 0;

	//column4.x = 0;
	//column4.y = 0;
	//column4.z = 0;(column1, column2, column3, column4);
	//column4.w = 1;
	

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixLookAtLH(DirectX::SimpleMath::Vector3(10, 20, 10), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 1, 0));

	DirectX::XMFLOAT3 finRot(0, 0, 0);
	DirectX::XMVECTOR orientation = DirectX::XMQuaternionRotationMatrix(DirectX::XMMatrixInverse(nullptr, rotationMatrix));
	DirectX::XMStoreFloat3(&finRot, orientation);

	


	spaceShip = new SpaceShip(DirectX::SimpleMath::Vector3(10, 20, 10), DirectX::SimpleMath::Vector3(finRot.x, finRot.y, finRot.z), 1);


	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	testCube->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));
	testCube->getPhysComp()->setParent(testCube);

	gameObjects.emplace_back(player);
	gameObjects.emplace_back(planet);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testCube);
	gameObjects.emplace_back(spaceShip);

	for (int i = 0; i < 10; i++)
	{
		GameObject* newObj = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		physWolrd.addPhysComponent(newObj, reactphysics3d::CollisionShapeName::BOX);
		newObj->getPhysComp()->setPosition(reactphysics3d::Vector3(-100, 120+(float)i*10, 100));
		gameObjects.emplace_back(newObj);
	}

	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	
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
		gameObjects.at(i)->updateBuffer();
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

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpWireframe();
	//camera.updateCamera(immediateContext);
	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(20, 30, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(10, -20, 30), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2,5)));


	this->mouse = &mouse;
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
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
	mouse->handleEvents(this->window, camera);

	//Do we want this?
	grav = planetGravityField.calcGravFactor(player->getPosV3());
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(player->getPos()));
	player->move(grav, camera.getRightVec(), dt);
	
	//Keeps player at the surface of the planet
	if (getLength(player->getPos()) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); DirectX::XMFLOAT3 tempPos = normalizeXMFLOAT3(player->getPos()); player->setPos(getScalarMultiplicationXMFLOAT3(22, tempPos)); }
	player->movePos(getScalarMultiplicationXMFLOAT3(dt, velocity));
	
	//Player functions
	player->pickupItem(potion);
	player->update();
	
	camera.moveCamera(player->getPosV3(), dt);
	
	physWolrd.updatePlayerBox(player->getPos());
	physWolrd.addForceToObjects();
	physWolrd.update(dt);
	potion->update(); //getPhysComp()->updateParent();
	testCube->update();//getPhysComp()->updateParent();

	//Here you can write client-server related functions?


	this->updateBuffers();
	
	if (player->repairedShip()) { std::cout << "You have repaired the ship and returned to earth\n"; return EXIT; }
	
	mouse->clearEvents();

	//Debug keybinds
	this->handleKeybinds();
	return NOCHANGE;
}

void Game::Render()
{
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
	start = std::chrono::system_clock::now();


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


	//Render Imgui & wireframe
	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (wireframe) { immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); }


	//Render Particles
	basicRenderer.geometryPass(this->camera);
	drawParticles();
	this->ptEmitters.at(0).unbind();
}

