#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"
#include "MemoryLeackChecker.h"

void Game::loadObjects()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	//Variables 
	GameObject* planet;
	SpaceShip* spaceShipRed;
	SpaceShip* spaceShipBlue;
	Potion* potion;
	BaseballBat* testBat;
	Grenade* grenade;
	Component* component;

	//Load extra textures
	MaterialLibrary::LoadDefault();
	MaterialLibrary::LoadMaterial("spaceshipTexture1.jpg");
	MaterialLibrary::LoadMaterial("spaceshipTexture2.jpg");
	MaterialLibrary::LoadMaterial("pintoRed.png");
	MaterialLibrary::LoadMaterial("pintoBlue.png");
	MaterialLibrary::LoadMaterial("Red.png");

	//Meshes vector contents
	//Sphere, pinto, potion, rocket, rocket, bat, Player, component, grenade
	meshes.push_back(new Mesh("../Meshes/Sphere"));
	meshes.push_back(new Mesh("../Meshes/pinto"));
	meshes.push_back(new Mesh("../Meshes/potion"));
	meshes.push_back(new Mesh("../Meshes/rocket"));
	meshes.push_back(new Mesh("../Meshes/bat"));
	meshes.push_back(new Mesh("../Meshes/component"));
	meshes.push_back(new Mesh("../Meshes/grenade"));

	manager.loadMeshAndBoneData("../Meshes/pinto_Run.fbx");

	this->manager.getAnimData("../Meshes/pinto_Run.fbx", vBuff, iBuff, subMeshRanges, verticies, animData);
	this->tmpMesh = new Mesh(vBuff, iBuff, subMeshRanges, verticies);
	this->currentPlayer = new Player(tmpMesh, DirectX::SimpleMath::Vector3(0, 42, 0), DirectX::SimpleMath::Vector3(0, 0, 0), PLAYER, client, 0, &planetGravityField);
	this->sexyMan->addData(animData);
	sexyMan->setSrv(this->manager.getSrv("../Textures/texture2.png"));
	//physWolrd.addPhysComponent(sexyMan, reactphysics3d::CollisionShapeName::BOX);

	planet = new GameObject(meshes[0], Vector3(0, 0, 0), Vector3(0.0f, 0.0f, 0.0f), PLANET, nullptr, XMFLOAT3(40.0f, 40.0f, 40.0f));
	physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::SPHERE, planet->getScale());
	planet->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	gameObjects.emplace_back(planet);
	if (IFONLINE) return;

	//Here we can add base object we want in the beginning of the game
	atmosphere = new GameObject(meshes[0], Vector3(0, 0, 0), Vector3(0.0f, 0.0f, 0.0f), PLANET, nullptr, XMFLOAT3(43, 43, 43));
	currentPlayer = new Player(meshes[1], Vector3(0, 48, 0), Vector3(0.0f, 0.0f, 0.0f), PLAYER, client, 0, &planetGravityField);
	potion = new Potion(meshes[2], Vector3(10, 10, 15),Vector3(0.0f, 0.0f, 0.0f), POTION, 0, &planetGravityField);
	spaceShipRed = new SpaceShip(meshes[3], Vector3(-7.81178f, -37.8586f, -8.50119f), ROCKET, 0, &planetGravityField, Vector3(2, 2, 2));
	spaceShipBlue = new SpaceShip(meshes[3], Vector3(13.5817f, 35.9383f, 9.91351f), ROCKET, 1, &planetGravityField, Vector3(2, 2, 2));
	testBat = new BaseballBat(meshes[4], Vector3(-10, 10, 15), Vector3(0.0f, 0.0f, 0.0f), BAT, 0, &planetGravityField);
	component = new Component(meshes[5], DirectX::SimpleMath::Vector3(10, -10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), COMPONENT, 0, &planetGravityField);
	grenade = new Grenade(meshes[6], DirectX::SimpleMath::Vector3(-10, -10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), GRENADE, 0, &planetGravityField);



	physWolrd.addPhysComponent(testBat, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(grenade, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(component, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(spaceShipRed, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(spaceShipBlue, reactphysics3d::CollisionShapeName::BOX);


	spaceShipRed->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	spaceShipBlue->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	potion->setRot(DirectX::SimpleMath::Vector3(0.0f, 1.5f, 1.5f));
	testBat->getPhysComp()->setPosition(reactphysics3d::Vector3(testBat->getPosV3().x, testBat->getPosV3().y, testBat->getPosV3().z));
	currentPlayer->setPhysComp(physWolrd.getPlayerBox());
	currentPlayer->getPhysComp()->setParent(currentPlayer);

	//Add to obj array
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testBat);
	gameObjects.emplace_back(grenade);
	//gameObjects.emplace_back(currentPlayer);
	gameObjects.emplace_back(component);
	gameObjects.emplace_back(spaceShipRed);
	gameObjects.emplace_back(spaceShipBlue);

	//Add to Item array
	items.emplace_back(potion);
	items.emplace_back(testBat);
	items.emplace_back(grenade);

	//Add to Paceship array
	spaceShips.emplace_back(spaceShipRed);
	spaceShips.emplace_back(spaceShipBlue);

	//Add to component array
	components.emplace_back(component);

	//Set active players/game objects for objects
	testBat->setPlayer(currentPlayer);
	testBat->setTestObj(gameObjects);
	testBat->setGameObjects(players);
	testBat->setClient(client);
	grenade->setGameObjects(gameObjects);
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
		gameObjects[i]->draw();
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

void Game::drawIcons()
{
	for (int i = 0; i < items.size(); i++)
	{
		this->items[i]->drawIcon();
	}
	for (int i = 0; i < components.size(); i++)
	{
		this->components[i]->drawIcon();
	}
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->drawIcon();
	}
	for (int i = 0; i < spaceShips.size(); i++)
	{
		spaceShips[i]->drawQuad();
	}
}

void Game::drawParticles()
{
	for (int i = 0; i < items.size(); i++)
	{
		this->items[i]->drawParticles();
	}
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->drawParticles();
	}
	for (int i = 0; i < components.size(); i++)
	{
		this->components[i]->drawParticles();
	}
	for (int i = 0; i < spaceShips.size(); i++)
	{
		spaceShips[i]->drawParticles();
	}
}

void Game::drawFresnel()
{
	GPU::immediateContext->PSSetConstantBuffers(2, 1, colorBuffer.getReferenceOf());
	atmosphere->draw();
	for (int i = 0; i < items.size(); i++)
	{
		if(items[i]->getId() == ObjID::GRENADE)
		{
			Grenade* tempNade = (Grenade*)items[i];
			tempNade->drawFresnel();
		}
	}
}

void Game::randomizeObjectPos(GameObject* object)
{
	DirectX::SimpleMath::Vector3 randomPos = {};

	int xPos = rand() % 201 - 100;
	int yPos = rand() % 201 - 100;
	int zPos = rand() % 201 - 100;

	randomPos.x = (float)xPos;
	randomPos.y = (float)yPos;
	randomPos.z = (float)zPos;

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
	//Update Wireframe buffer
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(wireBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &reactWireframeInfo, sizeof(wirefameInfo));
	immediateContext->Unmap(wireBuffer, 0);
}

void Game::handleKeybinds()
{
	if (GetAsyncKeyState('C')) physWolrd.addBoxToWorld();
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
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0)), endTimer(0), manager(ModelManager(device))
{
	//Setup client
	this->packetEventManager = new PacketEventManager();
	//m�ste raderas******************
	client = new Client();//("192.168.43.216");
	circularBuffer = client->getCircularBuffer();
	
	//Setup rendering
	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->setUpWireframe();

	//Setup Lights
	ltHandler.addLight(DirectX::XMFLOAT3(-90, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 1, 0),1);
	ltHandler.addLight(DirectX::XMFLOAT3(16 + 7, 42 + 17, 12 + 7), DirectX::XMFLOAT3(0, 0.3f, 1.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	ltHandler.addLight(DirectX::XMFLOAT3(-10 - 5, -45 - 17, -10 - 7), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);

	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(0.0f, 0.55f, 0.75f, 3.8f);
	this->colorBuffer.applyData();

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
		int dude = (NROFPLAYERS) / 2;

		int newNrOfPlayer = NROFPLAYERS;
		int tempTeam = 0;
		for (int i = 0; i < NROFPLAYERS; i++)//initialize players 
		{

			tempTeam = i < (newNrOfPlayer / 2);
			Player* tmpPlayer = nullptr;
			if (playerId != i)
			{
				tmpPlayer = new Player(meshes[2], DirectX::SimpleMath::Vector3(35.f + (float)(offset * i), 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, client, (int)(dude < i + 1), &planetGravityField);
				tmpPlayer->setOnlineID(i);
				physWolrd.addPhysComponent(tmpPlayer, reactphysics3d::CollisionShapeName::BOX);
				players.push_back(tmpPlayer);
			}
			else
			{
				currentPlayer = new Player(meshes[2], DirectX::SimpleMath::Vector3(0, 40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, client, (int)(dude < i+1), &planetGravityField);
				currentPlayer->setOnlineID(i);
				players.push_back(currentPlayer);
				delete tmpPlayer;
			}
			std::cout << "Dude: " << (int)(dude < i + 1) << "\n";
		}
	}

	//Load game objects
	this->loadObjects();


	//Init delta time
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
	delete atmosphere;

	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		delete this->gameObjects.at(i);
	}
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	wireBuffer->Release();

	delete tmpMesh;
	delete sexyMan;
}

GAMESTATE Game::Update()
{
	//read the packets received from the server
	packetEventManager->PacketHandleEvents(circularBuffer, NROFPLAYERS, players, client->getPlayerId(), components, physWolrd, gameObjects, &planetGravityField, spaceShips, onlineItems);
	
	//Get newest delta time
	lastUpdate = currentTime;
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();

	//Calculate gravity factor
	grav = planetGravityField.calcGravFactor(currentPlayer->getPosV3());
	additionXMFLOAT3(velocity, getScalarMultiplicationXMFLOAT3(dt, grav));

	//Raycasting
	static DirectX::XMFLOAT3 hitPos;
	static DirectX::XMFLOAT3 hitNormal;
	hitPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	hitNormal = DirectX::XMFLOAT3(grav.x, grav.y, grav.z);
	bool testingVec = this->currentPlayer->raycast(gameObjects, hitPos, hitNormal);
	if (testingVec || currentPlayer->getHitByBat()) velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	
	//Player functions
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), hitNormal, dt, testingVec);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, gamePad, dt);
	currentPlayer->movePos(velocity);
	currentPlayer->checkForStaticCollision(gameObjects);

	//Check component pickup
	for (int i = 0; i < components.size(); i++)
	{
		if (currentPlayer->pickupItem(components[i])) break;
	}
	//Check item pickup
	for (int i = 0; i < items.size(); i++)
	{
		if (currentPlayer->pickupItem(items[i])) break;
	}

	//Update item checks
	currentPlayer->checkMovement();
	for (int i = 0; i < items.size(); i++)
	{
		int id = items[i]->getId();
		switch (id)
		{
			case ObjID::GRENADE:
			{
				Grenade* tempNade = (Grenade*)items[i];
				tempNade->updateExplosionCheck();
			}	break;
			case ObjID::POTION:
			{
				Potion* tempPotion = (Potion*)items[i];
				if (tempPotion->isTimeToRun())
				{
					if (tempPotion->timerGoing()) currentPlayer->setSpeed(50.f);
					else currentPlayer->setSpeed(25.f);
				}
			}	break;
		}
	}


	//sending data to server
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		//client->sendToServerTEMPTCP(currentPlayer);
		
		SendingDataEvent(client, currentPlayer, players);
		serverStart = std::chrono::system_clock::now();
	}

	
	//Physics related functions
	if (!IFONLINE) physWolrd.update(dt);
	for (int i = 0; i < players.size(); i++)
	{
		
		players[i]->updateMatrixOnline();
		players[i]->update();
	}
	currentPlayer->updateBuffer();

	//Updates gameObject physics components
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
	
	//Setting the camera at position
	camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX());

	//Check Components online
	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (spaceShips[i]->getCompletion())
		{
			if (currentPlayer->getTeam() ==  i) 
			{
				camera.winScene(spaceShips[i]->getPosV3(), spaceShips[i]->getRot());
			}
			grav = planetGravityField.calcGravFactor(this->spaceShips[i]->getPosV3());
			this->spaceShips[i]->move(grav, dt);
			endTimer += dt;
		}
	}
	if (!IFONLINE) //Check Components offline
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			for (int j = 0; j < components.size(); j++)
			{
				if (spaceShips[i]->detectedComponent(components[j]))
				{
					currentPlayer->releaseItem();
					randomizeObjectPos(components[j]);
					spaceShips[i]->addComponent();
					spaceShips[i]->setAnimate(true);
				}
			}
		}
	}
	//Check winstate
	if (endTimer > 6)
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			if (spaceShips[i]->isFinished())
			{
				int team = spaceShips[i]->getTeam();
				if (currentPlayer->getTeam() == team)
				{
					return WIN;
				}
				else
				{
					return LOSE;
				}
			}
		}
	}

	//Update Line rendering buffer
	this->updateBuffers();

	//Play pickup animation
	for (int i = 0; i < spaceShips.size(); i++)
	{
		spaceShips[i]->animateOnPickup();
	}

	//Updates gameObject physics components
	this->updateBuffers();
	
	//Check if item icon should change to pickup icon 
	for (int i = 0; i < items.size(); i++)
	{
		this->items[i]->checkDistance((GameObject*)(currentPlayer));
	}
	for (int i = 0; i < components.size(); i++)
	{
		this->components[i]->checkDistance((GameObject*)(currentPlayer));
	}

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

	//Render fresnel objects
	basicRenderer.fresnelPrePass(this->camera);
	this->drawFresnel();

	basicRenderer.changeToAnimation();
	this->sexyMan->draw(dt, 0);
	currentPlayer->draw(dt, 0);

	//Render Skybox
	basicRenderer.skyboxPrePass();
	skybox.draw();
	basicRenderer.depthUnbind();

	//Render imgui & wireframe
	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (wireframe) { immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); playerVecRenderer.drawLines(); }

	//render billboard objects
	basicRenderer.bilboardPrePass(this->camera);
	drawIcons();

	//Render Particles
	basicRenderer.geometryPass(this->camera);
	drawParticles();
	basicRenderer.geometryUnbind();

	//Render UI (needs to render last)
	ui.Draw();
}

