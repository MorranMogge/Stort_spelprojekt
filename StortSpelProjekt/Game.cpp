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
	spaceShip = new SpaceShip(DirectX::SimpleMath::Vector3(10, 14, 10), orientToPlanet(DirectX::SimpleMath::Vector3(10, 20, 10)), 1, DirectX::SimpleMath::Vector3(2, 2, 2));


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

DirectX::SimpleMath::Vector3 Game::orientToPlanet(DirectX::XMFLOAT3 position)
{
	using namespace DirectX;
	DirectX::SimpleMath::Vector3 downVector = planetGravityField.calcGravFactor(position);
	downVector *= -1;
	downVector.Normalize();

	XMMATRIX rotation = XMMatrixIdentity();


	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	
	DirectX::XMVECTOR upVector = downVector;
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;
	
	bool rotationDone = false;

	XMVECTOR dotProduct;
	XMFLOAT3 dotValue;
	XMFLOAT3 dotValue2;

	/*	DirectX::SimpleMath::Vector3 downVector = planetGravityField.calcGravFactor(DirectX::SimpleMath::Vector3(10, 20, 10));
	downVector *= -1;
	downVector.Normalize();

	float angle = std::atan2(downVector.y, downVector.x);
	DirectX::SimpleMath::Matrix glmrotXY = DirectX::SimpleMath::Matrix::CreateRotationZ(angle);

	float angleZ = -std::asin(downVector.z);
	DirectX::SimpleMath::Matrix glmrotZ = DirectX::SimpleMath::Matrix::CreateRotationY(angleZ);

	DirectX::SimpleMath::Matrix rotation = glmrotXY* glmrotZ;
	DirectX::SimpleMath::Quaternion qua = DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(rotation);
	DirectX::SimpleMath::Vector3 rot = qua.ToEuler();*/

	//X rotation

	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	dotProduct = DirectX::XMVector3Dot(upVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);

	DirectX::SimpleMath::Matrix z = DirectX::XMMatrixRotationAxis(forwardVector, -std::atan(dotValue.z));
	DirectX::SimpleMath::Matrix x = DirectX::XMMatrixRotationAxis(rightVector, std::asin(dotValue.x));
	DirectX::SimpleMath::Matrix y = DirectX::XMMatrixRotationAxis(upVector, std::asin(dotValue.y));
	DirectX::SimpleMath::Matrix f = z * x * y;

	DirectX::SimpleMath::Quaternion test = DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(f);
	DirectX::SimpleMath::Vector3 finalRotation = test.ToEuler();

	return finalRotation;
}


/*DirectX::SimpleMath::Vector3 Game::orientToPlanet(DirectX::XMFLOAT3 position)
{
	using namespace DirectX;
	DirectX::SimpleMath::Vector3 downVector = planetGravityField.calcGravFactor(position);
	downVector *= -1;
	downVector.Normalize();

	XMMATRIX rotation = XMMatrixIdentity();


	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	
	DirectX::XMVECTOR upVector = downVector;
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;
	
	bool rotationDone = false;

	XMVECTOR dotProduct;
	XMFLOAT3 dotValue;
	XMFLOAT3 dotValue2;



	//X rotation
while (!rotationDone)
{
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);

	dotProduct = DirectX::XMVector3Dot(upVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);

	if (dotValue.x < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, -0.1f);
	}
	else if (dotValue.x > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, 0.1f);
	}

	dotProduct = DirectX::XMVector3Dot(upVector, rightVector);
	XMStoreFloat3(&dotValue2, dotProduct);
	if (dotValue2.z < -0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, 0.1f);
	}
	else if (dotValue2.z > 0.1f)
	{
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -0.1f);
	}

	if ((dotValue.x >= -0.1f && dotValue.x <= 0.1f) && (dotValue2.x >= -0.1f && dotValue2.x <= 0.1f))
	{
		rotationDone = true;
	}
}

DirectX::SimpleMath::Quaternion test = DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(rotation);
DirectX::SimpleMath::Vector3 finalRotation = test.ToEuler();

return finalRotation;
}*/


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
	

	//spaceShip->setRot(DirectX::XMFLOAT3(spaceShip->getRot().x, spaceShip->getRot().y +0.001, spaceShip->getRot().z + 0.001));

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

