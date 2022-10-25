#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"



void Game::loadObjects()
{
	float planetSize = 40.f;
	grav = planetGravityField.calcGravFactor(DirectX::SimpleMath::Vector3(0, 40, 0));

	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Sphere", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, DirectX::XMFLOAT3(planetSize, planetSize, planetSize));
	currentPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(0, 40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, grav);
	potion = new Potion("../Meshes/Baseball", DirectX::SimpleMath::Vector3(40, 40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 2);
	spaceShip = new SpaceShip(DirectX::SimpleMath::Vector3(20, 29, 20), orientToPlanet(DirectX::SimpleMath::Vector3(0, 40, 0)), 3, DirectX::SimpleMath::Vector3(2, 2, 2));
	testBat = new BaseballBat("../Meshes/Baseball", DirectX::SimpleMath::Vector3(-10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 4);
	testCube = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 5, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	otherPlayer = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(-22, 12, 22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 6, grav);
	component = new Component("../Meshes/Baseball", DirectX::SimpleMath::Vector3(10, -10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 2);

	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(testBat, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(otherPlayer, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(component, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::SPHERE, planet->getScale());
	physWolrd.addPhysComponent(spaceShip, reactphysics3d::CollisionShapeName::BOX, DirectX::XMFLOAT3(0.75f, 3*0.75f, 0.75f));
	potion->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	planet->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	spaceShip->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	spaceShip->updatePhysCompRotation();

	spaceShip->getPhysComp()->setPosition(reactphysics3d::Vector3(spaceShip->getPosV3().x, spaceShip->getPosV3().y, spaceShip->getPosV3().z));
	testCube->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	potion->setRot(DirectX::SimpleMath::Vector3(0.0f, 1.5f, 1.5f));
	testBat->getPhysComp()->setPosition(reactphysics3d::Vector3(testBat->getPosV3().x, testBat->getPosV3().y, testBat->getPosV3().z));
	otherPlayer->getPhysComp()->setPosition(reactphysics3d::Vector3(otherPlayer->getPosV3().x, otherPlayer->getPosV3().y, otherPlayer->getPosV3().z));

	gameObjects.emplace_back(currentPlayer);
	gameObjects.emplace_back(spaceShip);
	gameObjects.emplace_back(planet);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testCube);
	gameObjects.emplace_back(testBat);
	gameObjects.emplace_back(otherPlayer);
	gameObjects.emplace_back(component);


	for (int i = 0; i < 10; i++)
	{
		GameObject* newObj = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 6+ i, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		physWolrd.addPhysComponent(newObj, reactphysics3d::CollisionShapeName::BOX);
		newObj->getPhysComp()->setPosition(reactphysics3d::Vector3(-100, 120+(float)i*10, 100));
		gameObjects.emplace_back(newObj);
	}

	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	testBat->setPlayer(currentPlayer);
	testBat->setTestObj(gameObjects);
	currentPlayer->setPhysComp(physWolrd.getPlayerBox());
	currentPlayer->getPhysComp()->setParent(currentPlayer);
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

	currentTime = std::chrono::system_clock::now();
	lastUpdate = currentTime;
	gamePad = std::make_unique<DirectX::GamePad>();
	playerVecRenderer.setPlayer(currentPlayer);
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();
	serverStart = std::chrono::system_clock::now();
	this->window = &window;

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
			Player* tmpPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, grav);
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
	//Get newest delta time
	lastUpdate = currentTime;
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();

	//Calculate gravity factor
	grav = planetGravityField.calcGravFactor(currentPlayer->getPosV3());
	additionXMFLOAT3(velocity, getScalarMultiplicationXMFLOAT3(dt, grav));

	//Player functions
	//Raycasting
	DirectX::XMFLOAT3 hitPos(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 hitNormal(grav.x, grav.y, grav.z);
	bool testingVec = this->currentPlayer->raycast(gameObjects, hitPos, hitNormal);
	if (testingVec || currentPlayer->getHitByBat()) velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), hitNormal, dt, testingVec);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, gamePad, dt);
	currentPlayer->movePos(velocity);
	currentPlayer->checkForStaticCollision(gameObjects);

	currentPlayer->pickupItem(potion);
	currentPlayer->pickupItem(testBat);
	currentPlayer->pickupItem(component);


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
	
	//Physics related functions
	physWolrd.update(dt);

	camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), dt);
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
	//Here you can write client-server related functions?

	//Updates gameObject physics components
	camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), dt);
	for (int i = 1; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->getId() != this->spaceShip->getId())
		{
			gameObjects[i]->update();
		}
		
	}

	//Updates gameObject buffers
	this->updateBuffers();
	
	//Check winstate
	for (int i = 1; i < gameObjects.size(); i++)
	{
		Component* comp = dynamic_cast<Component*>(gameObjects[i]);
		if (comp && spaceShip->detectedComponent(comp))
		{
			std::cout << "Detected Component!\nID: " << comp->getId() << "\n";
		}
	}
	if (currentPlayer->repairedShip()) { std::cout << "You have repaired the ship and returned to earth\n"; return EXIT; }
	
	//Debug keybinds
	this->handleKeybinds();
	return NOCHANGE;
}

void Game::Render()
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
	}
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

