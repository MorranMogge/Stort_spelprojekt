#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"

void Game::loadObjects()
{

	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	float planetSize = 40.f;

	//Here we can add base object we want in the beginning of the game
	currentPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(0, 40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, client, &planetGravityField);
	
	planet = new GameObject("../Meshes/Sphere", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, nullptr, DirectX::XMFLOAT3(planetSize, planetSize, planetSize));
	potion = new Potion("../Meshes/potion", DirectX::SimpleMath::Vector3(0, -40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 2, 0, &planetGravityField);
	testBat = new BaseballBat("../Meshes/bat", DirectX::SimpleMath::Vector3(0, 40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 4, 0, &planetGravityField);
	grenade = new Grenade("../Meshes/grenade", DirectX::SimpleMath::Vector3(40, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 6, GRENADE, &planetGravityField);

	physWolrd.addPhysComponent(testBat, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	
	
	physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::SPHERE, planet->getScale());
	physWolrd.addPhysComponent(grenade, reactphysics3d::CollisionShapeName::BOX);
	planet->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	potion->setRot(DirectX::SimpleMath::Vector3(0.0f, 1.5f, 1.5f));
	testBat->getPhysComp()->setPosition(reactphysics3d::Vector3(testBat->getPosV3().x, testBat->getPosV3().y, testBat->getPosV3().z));

	gameObjects.emplace_back(planet);
	gameObjects.emplace_back(currentPlayer);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testBat);
	gameObjects.emplace_back(grenade);
	
	items.emplace_back(potion);
	items.emplace_back(testBat);
	items.emplace_back(grenade);

	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	testBat->setPlayer(currentPlayer);
	testBat->setTestObj(gameObjects);
	grenade->setGameObjects(gameObjects);
	currentPlayer->setPhysComp(physWolrd.getPlayerBox());
	currentPlayer->getPhysComp()->setParent(currentPlayer);
}

void Game::drawShadows()
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
	}
	for (int i = 0; i < ltHandler.getNrOfLights(); i++)
	{
		ltHandler.drawShadows(i, gameObjects);
	}
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->draw();
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
		gameObjects[i]->draw();
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
			this->ptEmitters.at(i).BindAndDraw(0);
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
	randomPos *= 100;

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
	
	for (int i = 0; i < onlineItems.size(); i++)
	{
		onlineItems[i]->updateBuffer();
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

DirectX::SimpleMath::Vector3 Game::orientToPlanet(const DirectX::XMFLOAT3& position)
{
	using namespace DirectX; using namespace SimpleMath;

	//Modified vectors
	XMVECTOR upVector = (planetGravityField.calcGravFactor(position) * -1);
	XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR rightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	Vector3 resultVector;
	XMMATRIX rotation = XMMatrixIdentity();

	//X-Rotation
	resultVector = DirectX::XMVector3Dot(upVector, forwardVector);
	if (resultVector.x < 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(rightVector, upVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(forwardVector, resultVector);
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, -resultVector.x);
	}
	else if (resultVector.x > 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(rightVector, upVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(forwardVector, resultVector);
		rotation *= DirectX::XMMatrixRotationAxis(rightVector, resultVector.x);
	}

	//Z-Rotation
	resultVector = DirectX::XMVector3Dot(upVector, rightVector);
	if (resultVector.z < 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(upVector, forwardVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(rightVector, resultVector);
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, resultVector.z);
	}
	else if (resultVector.z > 0.0f)
	{
		resultVector = DirectX::XMVector3Cross(upVector, forwardVector);
		resultVector = DirectX::XMVector3Normalize(resultVector);
		resultVector = DirectX::XMVector3AngleBetweenNormals(rightVector, resultVector);
		rotation *= DirectX::XMMatrixRotationAxis(forwardVector, -resultVector.z);
	}

	//Extracting rotation
	Quaternion quaterRot = Quaternion::CreateFromRotationMatrix(rotation);
	Vector3 finalRotation = quaterRot.ToEuler();

	return finalRotation;
}


Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	MaterialLibrary::LoadDefault();
	MaterialLibrary::LoadMaterial("spaceshipTexture1.jpg");

	this->packetEventManager = new PacketEventManager();
	//m�ste raderas******************
	client = new Client("192.168.43.251");
	circularBuffer = client->getCircularBuffer();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpWireframe();

	//camera.updateCamera(immediateContext);
	ltHandler.addLight(DirectX::XMFLOAT3(-90, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 1);
	ltHandler.addLight(DirectX::XMFLOAT3(50, 30, 0), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 0);
	ltHandler.addLight(DirectX::XMFLOAT3(10, -50, 30), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);

	//Particle test
	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2, 5)));

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
		int offset = 10;
		for (int i = 0; i < NROFPLAYERS; i++)//initialize players 
		{
			Player* tmpPlayer = nullptr;
			if (i <= (NROFPLAYERS / 2))//team 1
			{
				tmpPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(35 + (offset * i), 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, client, &planetGravityField);
			}
			else//team 2
			{
				tmpPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(35 + (offset * i), 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, client, &planetGravityField);
			}
			if (playerId != i)
			{
				tmpPlayer->setOnlineID(i);
				physWolrd.addPhysComponent(tmpPlayer, reactphysics3d::CollisionShapeName::BOX);
				players.push_back(tmpPlayer);
			}
			else
			{
				currentPlayer->setOnlineID(i);
				players.push_back(currentPlayer);
				delete tmpPlayer;
			}
		}
	}
	testBat->setGameObjects(players);
	testBat->setClient(client);

	currentTime = std::chrono::system_clock::now();
	lastUpdate = currentTime;
	gamePad = std::make_unique<DirectX::GamePad>();
	playerVecRenderer.setPlayer(currentPlayer);
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();
	serverStart = std::chrono::system_clock::now();
	this->window = &window;
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
	static DirectX::XMFLOAT3 hitPos;
	static DirectX::XMFLOAT3 hitNormal;
	hitPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	hitNormal = DirectX::XMFLOAT3(grav.x, grav.y, grav.z);
	bool testingVec = this->currentPlayer->raycast(gameObjects, hitPos, hitNormal);
	if (testingVec || currentPlayer->getHitByBat()) velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), hitNormal, dt, testingVec);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, gamePad, dt);
	currentPlayer->movePos(velocity);
	currentPlayer->checkForStaticCollision(gameObjects);

	for (int i = 0; i < components.size(); i++)
	{
		if (currentPlayer->pickupItem(components[i])) break;
	}

	for (int i = 0; i < items.size(); i++)
	{
		if (currentPlayer->pickupItem(items[i])) break;
	}

	if (Input::KeyPress(KeyCode::K))
	{
		randomizeObjectPos(this->testBat);
	}

	grenade->updateExplosionCheck();
	if (potion->isTimeToRun()) currentPlayer->setSpeed(50.f);
	else currentPlayer->setSpeed(25.f);

	//sending data to server
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		//client->sendToServerTEMPTCP(currentPlayer);
		
		SendingDataEvent(client, currentPlayer, players);
		serverStart = std::chrono::system_clock::now();
	}

	//read the packets received from the server
	packetEventManager->PacketHandleEvents(circularBuffer, NROFPLAYERS, players, client->getPlayerId(), components, physWolrd, gameObjects, &planetGravityField, spaceShips, onlineItems);
	
	
	//Physics related functions
	if (!IFONLINE) physWolrd.update(dt);
	
	for (int i = 0; i < players.size(); i++)
	{
		//players[i]->updateBuffer();
		
		players[i]->updateMatrixOnline();
		players[i]->update();
	}

	for (int i = 0; i < onlineItems.size(); i++)
	{
		onlineItems[i]->update();
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
	
	//Setting the camera at position
	bool noWinners = true;
	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (spaceShips[i]->getCompletion())
		{
			camera.winScene(spaceShips[i]->getPosV3(), spaceShips[i]->getRot());
			grav = planetGravityField.calcGravFactor(this->spaceShips[i]->getPosV3());
			this->spaceShips[i]->move(grav, dt);
			noWinners = false;
		}
	}
	if (noWinners) camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX());
	

	//Here you can write client-server related functions?

	//Updates gameObject physics components

	

	//Updates gameObject buffers
	this->updateBuffers();
	

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

	for (int i = 0; i < onlineItems.size(); i++)
	{
		onlineItems[i]->draw();
	}

	//Render imgui & wireframe
	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (wireframe) { immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); playerVecRenderer.drawLines(); }

	//render billboard objects
	basicRenderer.bilboardPrePass(this->camera);
	this->potion->drawIcon();
	this->testBat->drawIcon();
	this->grenade->drawIcon();
	this->currentPlayer->drawIcon(0);

	for (int i = 0; i < spaceShips.size(); i++)
	{
		spaceShips[i]->drawQuad();
	}
	//Render Particles
	basicRenderer.geometryPass(this->camera);
	//drawParticles();
	//this->ptEmitters.at(0).unbind();

	//drawParticles();	//not in use, intended for drawing particles in game.cpp
	this->potion->drawParticles();
	this->testBat->drawParticles();
	this->grenade->drawParticles();
	//for (int i = 0; i < players.size(); i++)
	//{
	//	players[i]->drawParticles();
	//	players[i]->drawIcon(players[i]->getOnlineID());
	//}
	this->currentPlayer->drawParticles();
	basicRenderer.geometryUnbind();


	//Render UI (needs to render last)
	ui.Draw();
}

