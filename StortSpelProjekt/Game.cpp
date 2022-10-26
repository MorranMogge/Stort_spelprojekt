#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"



void Game::loadObjects()
{
	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Sphere", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, DirectX::XMFLOAT3(20.0f, 20.0f, 20.0f));
	currentPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1);
	potion = new Potion("../Meshes/Baseball", DirectX::SimpleMath::Vector3(10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 2);
	spaceShip = new SpaceShip(DirectX::SimpleMath::Vector3(10, 14, 10), orientToPlanet(DirectX::SimpleMath::Vector3(10, 20, 10)), 3, DirectX::SimpleMath::Vector3(2, 2, 2));
	testBat = new BaseballBat("../Meshes/Baseball", DirectX::SimpleMath::Vector3(-10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 4);
	testCube = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 5, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	otherPlayer = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(-22, 12, 22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 6);

	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(testBat, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(otherPlayer, reactphysics3d::CollisionShapeName::BOX);

	testCube->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));
	
	randomizeObjectPos(potion);
	
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	testBat->getPhysComp()->setPosition(reactphysics3d::Vector3(testBat->getPosV3().x, testBat->getPosV3().y, testBat->getPosV3().z));
	otherPlayer->getPhysComp()->setPosition(reactphysics3d::Vector3(otherPlayer->getPosV3().x, otherPlayer->getPosV3().y, otherPlayer->getPosV3().z));

	gameObjects.emplace_back(planet);
	gameObjects.emplace_back(currentPlayer);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(spaceShip);
	gameObjects.emplace_back(testCube);
	gameObjects.emplace_back(testBat);
	gameObjects.emplace_back(otherPlayer);

	//for (int i = 0; i < 10; i++)
	//{
	//	GameObject* newObj = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 6+ i, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	//	physWolrd.addPhysComponent(newObj, reactphysics3d::CollisionShapeName::BOX);
	//	newObj->getPhysComp()->setPosition(reactphysics3d::Vector3(-100, 120+(float)i*10, 100));
	//	gameObjects.emplace_back(newObj);
	//}

	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	testBat->setPlayer(currentPlayer);
	testBat->setTestObj(gameObjects);
	currentPlayer->setPhysComp(physWolrd.getPlayerBox());

}

void Game::drawShadows()
{
	potion->draw();
	currentPlayer->draw();
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
	}
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
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
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

void Game::randomizeObjectPos(GameObject* object)
{
	DirectX::SimpleMath::Vector3 randomPos = {};

	int xPos = rand() % 201 - 100;
	int yPos = rand() % 201 - 100;
	int zPos = rand() % 201 - 100;

	randomPos.x = xPos;
	randomPos.y = yPos;
	randomPos.z = zPos;

	randomPos.Normalize();
	randomPos *= 30;

	object->setPos(randomPos);
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
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->updateBuffer();
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
	if (Input::KeyPress(KeyCode::K))
	{
		randomizeObjectPos(potion);
	}
}


DirectX::SimpleMath::Vector3 Game::orientToPlanet(const DirectX::XMFLOAT3 &position)
{
	using namespace DirectX; using namespace SimpleMath;

	//Default vectors
	const XMVECTOR DEFAULT_RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	
	//Modified vectors
	XMVECTOR upVector = (planetGravityField.calcGravFactor(position) * -1);
	XMVECTOR forwardVector = DEFAULT_FORWARD;
	XMVECTOR rightVector = DEFAULT_RIGHT;
	XMVECTOR dotProduct;
	XMFLOAT3 dotValue;

	//Rotation matrix
	XMMATRIX rotation = XMMatrixIdentity();

	//rotation
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	rightVector =	XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	rightVector =	XMVector3Normalize(rightVector);
	forwardVector = XMVector3Normalize(forwardVector);
	dotProduct = DirectX::XMVector3Dot(upVector, forwardVector);
	XMStoreFloat3(&dotValue, dotProduct);
	
	//creating matrix
	Matrix z = XMMatrixRotationAxis(forwardVector, -std::atan(dotValue.z));
	Matrix x = XMMatrixRotationAxis(rightVector, std::asin(dotValue.x));
	Matrix y = DirectX::XMMatrixRotationAxis(upVector, std::asin(dotValue.y));
	Matrix f = z * x * y;
	
	//Extracting rotation
	Quaternion quaterRot = Quaternion::CreateFromRotationMatrix(f);
	Vector3 finalRotation = quaterRot.ToEuler();

	return finalRotation;
}


Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	MaterialLibrary::LoadDefault();

	this->packetEventManager = new PacketEventManager();
	//m�ste raderas******************
	client = new Client("192.168.43.251");
	circularBuffer = client->getCircularBuffer();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpWireframe();

	//camera.updateCamera(immediateContext);
	ltHandler.addLight(DirectX::XMFLOAT3(-57, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0),1);
	ltHandler.addLight(DirectX::XMFLOAT3(20, 30, 0), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 0);
	ltHandler.addLight(DirectX::XMFLOAT3(10, -20, 30), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);

	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2,5)));

	serverStart = std::chrono::system_clock::now();
	this->window = &window;
	gamePad = std::make_unique<DirectX::GamePad>();
	playerVecRenderer.setPlayer(currentPlayer);
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();

	if (IFONLINE)
	{
		client->connectToServer();
		int playerId = -1;
		while (playerId <= -1 || playerId >= 9)
		{
			playerId = packetEventManager->handleId(client->getCircularBuffer());
			//std::cout << "Game.cpp, playerId: " << std::to_string(playerId) << std::endl;
		}
		//int playerid = client->initTEMPPLAYERS();

		this->client->setClientId(playerId);

		for (int i = 0; i < NROFPLAYERS; i++)//initialize players 
		{
			Player* tmpPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
			if (playerId != i)
			{
				players.push_back(tmpPlayer);
			}
			else
			{
				players.push_back(currentPlayer);
				delete tmpPlayer;
			}
		}
	}
}

Game::~Game()
{
	delete client;
	delete packetEventManager;

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

	//Do we want this?
	grav = planetGravityField.calcGravFactor(currentPlayer->getPosV3());
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(currentPlayer->getPos()));

	//Keeps currentPlayer at the surface of the planet
	if (getLength(currentPlayer->getPos()) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); DirectX::XMFLOAT3 tempPos = normalizeXMFLOAT3(currentPlayer->getPos()); currentPlayer->setPos(getScalarMultiplicationXMFLOAT3(22, tempPos)); }
	currentPlayer->movePos(getScalarMultiplicationXMFLOAT3(dt, velocity));

	//sending data to server
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		//client->sendToServerTEMPTCP(currentPlayer);
		SendingDataEvent(client, currentPlayer, players);
		serverStart = std::chrono::system_clock::now();
	}

	packetEventManager->PacketHandleEvents(circularBuffer, NROFPLAYERS, players, client->getPlayerId());
	
	
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->updateBuffer();
	}
	
	//Player, camera & physworld functions
	currentPlayer->pickupItem(potion);
	currentPlayer->pickupItem(testBat);
	physWolrd.updatePlayerBox(currentPlayer->getPos());
	physWolrd.addForceToObjects();
	physWolrd.update(dt);



	currentPlayer->update();
	//Updates gameObject physics components
	for (int i = 2; i < gameObjects.size(); i++)
	{
		if (gameObjects.at(i)->getId() != this->spaceShip->getId())
		{
			gameObjects[i]->update();//->getPhysComp()->updateParent();
		}
		
	}

	grav = normalizeXMFLOAT3(grav);
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, dt);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, gamePad, dt);
	camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), dt);
	
	
	camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), dt);
	/*if (Input::KeyDown(KeyCode::H))
	{
		camera.moveCamera(potion->getPosV3(), potion->getRot(), dt);
	}*/
	

	//Updates gameObject buffers
	this->updateBuffers();
	
	//Check winstate
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (i > 0 && spaceShip->detectedComponent(gameObjects.at(i)))
		{
			if (gameObjects.at(i)->getId() == this->testBat->getId())
			{
				std::cout << "detected: " << gameObjects.at(i)->getId() << std::endl;
				std::cout << "detected: Bat!" << std::endl;
			}
		}
	}
	if (currentPlayer->repairedShip()) { std::cout << "You have repaired the ship and returned to earth\n"; return EXIT; }
	
	//Debug keybinds
	this->handleKeybinds();
	return NOCHANGE;
}

void Game::Render()
{
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
	start = std::chrono::system_clock::now();

	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
	}

	basicRenderer.setUpScene();
	if (objectDraw) this->drawObjects(false);
	if (wireframe) { this->updateBuffers(); immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); }

	//imguiHelper->drawInterface("test");

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

