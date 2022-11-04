#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"

void Game::loadObjects()
{
	//Meshes vector contents
	//Sphere, pinto, potion, rocket, rocket, bat, Player, component, grenade

	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	float planetSize = 40.f;

	//Here we can add base object we want in the beginning of the game
	meshes.push_back(new Mesh("../Meshes/Sphere"));
	planet = new GameObject(meshes.back(), Vector3(0, 0, 0), Vector3(0.0f, 0.0f, 0.0f), PLANET, nullptr, XMFLOAT3(planetSize, planetSize, planetSize));
	atmosphere = new GameObject(meshes.back(), Vector3(0, 0, 0), Vector3(0.0f, 0.0f, 0.0f), PLANET, nullptr, XMFLOAT3(43, 43, 43));
	meshes.push_back(new Mesh("../Meshes/pinto"));
	currentPlayer = new Player(meshes.back(), Vector3(0, 48, 0), Vector3(0.0f, 0.0f, 0.0f), PLAYER, 0, &planetGravityField);
	meshes.push_back(new Mesh("../Meshes/potion"));
	potion = new Potion(meshes.back(), Vector3(10, 10, 15),Vector3(0.0f, 0.0f, 0.0f), POTION, &planetGravityField);
	meshes.push_back(new Mesh("../Meshes/rocket"));
	spaceShipRed = new SpaceShip(meshes.back(), Vector3(-7.81178f, -37.8586f, -8.50119f), ROCKET, 0, &planetGravityField, Vector3(2, 2, 2));
	//meshes.push_back(new Mesh("../Meshes/rocket"));
	spaceShipBlue = new SpaceShip(meshes.back(), Vector3(13.5817f, 35.9383f, 9.91351f), ROCKET, 1, &planetGravityField, Vector3(2, 2, 2));
	meshes.push_back(new Mesh("../Meshes/bat"));
	testBat = new BaseballBat(meshes.back(), Vector3(-10, 10, 15), Vector3(0.0f, 0.0f, 0.0f), BAT, &planetGravityField);
	meshes.push_back(new Mesh("../Meshes/Player"));
	testCube = new GameObject(meshes.back(), Vector3(0, 0, 0), Vector3(0.0f, 0.0f, 0.0f), MOVABLE, nullptr, XMFLOAT3(1.0f, 1.0f, 1.0f));
	//otherPlayer = new Player("../Meshes/Player", Vector3(-22, 12, 22), Vector3(0.0f, 0.0f, 0.0f), PLAYER, & planetGravityField);
	meshes.push_back(new Mesh("../Meshes/component"));
	component = new Component(meshes.back(), DirectX::SimpleMath::Vector3(10, -10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), COMPONENT, &planetGravityField);
	meshes.push_back(new Mesh("../Meshes/grenade"));
	grenade = new Grenade(meshes.back(), DirectX::SimpleMath::Vector3(-10, -10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), GRENADE, &planetGravityField);
	


	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(testBat, reactphysics3d::CollisionShapeName::BOX);
	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	
	
	physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::SPHERE, planet->getScale());
	physWolrd.addPhysComponent(grenade, reactphysics3d::CollisionShapeName::BOX);
	planet->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
	
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));
	potion->setRot(DirectX::SimpleMath::Vector3(0.0f, 1.5f, 1.5f));
	testBat->getPhysComp()->setPosition(reactphysics3d::Vector3(testBat->getPosV3().x, testBat->getPosV3().y, testBat->getPosV3().z));

	//Add to obj array
	gameObjects.emplace_back(planet);
	//gameObjects.emplace_back(currentPlayer);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testBat);
	gameObjects.emplace_back(grenade);


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
	/*if (Input::KeyPress(KeyCode::K))
	{
		randomizeObjectPos(potion);
	}*/
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
	MaterialLibrary::LoadMaterial("spaceshipTexture2.jpg");
	MaterialLibrary::LoadMaterial("pintoRed.png");
	MaterialLibrary::LoadMaterial("pintoBlue.png");
	MaterialLibrary::LoadMaterial("Red.png");

	this->packetEventManager = new PacketEventManager();
	//m�ste raderas******************
	client = new Client();//("192.168.43.216");
	circularBuffer = client->getCircularBuffer();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->setUpWireframe();

	//camera.updateCamera(immediateContext);
	ltHandler.addLight(DirectX::XMFLOAT3(-90, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 1, 0),1);
	ltHandler.addLight(DirectX::XMFLOAT3(16 + 7, 42 + 17, 12 + 7), DirectX::XMFLOAT3(0, 0.3f, 1.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	ltHandler.addLight(DirectX::XMFLOAT3(-10 - 5, -45 - 17, -10 - 7), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	
	//ltHandler.addLight(DirectX::XMFLOAT3(10, -50, 30), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);

	//Particle test
	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2, 5)));


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
	this->loadObjects();
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

	for (int i = 0; i < players.size(); i++)
	{
		delete players[i];
	}

	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	delete atmosphere;



	wireBuffer->Release();
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

	for (int i = 0; i < components.size(); i++)
	{
		if (currentPlayer->pickupItem(components[i])) break;
	}

	for (int i = 0; i < items.size(); i++)
	{
		if (currentPlayer->pickupItem(items[i])) break;
	}

	/*if (Input::KeyPress(KeyCode::K))
	{
		randomizeObjectPos(this->testBat);
	}*/
	currentPlayer->checkMovement();
	grenade->updateExplosionCheck();
	if (potion->isTimeToRun())
	{
		if (potion->timerGoing()) currentPlayer->setSpeed(50.f);
		else currentPlayer->setSpeed(25.f);
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
	bool noWinners = true;
	if (noWinners) camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX());
	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (spaceShips[i]->getCompletion())
		{
			if (currentPlayer->getTeam() ==  i) camera.winScene(spaceShips[i]->getPosV3(), spaceShips[i]->getRot());
			grav = planetGravityField.calcGravFactor(this->spaceShips[i]->getPosV3());
			this->spaceShips[i]->move(grav, dt);
			noWinners = true;
		}
	}

	//Applies updated movement & rotation to models
	this->updateBuffers();


	//Move camera
	camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), dt);


	//Check component pickup
	for (int i = 1; i < gameObjects.size(); i++)
	{
		//get object id
		int id = gameObjects.at(i)->getId();

		//If component
		if (id == COMPONENT)
		{
			//Check if RED spaceship detected
			if(spaceShipRed->detectedComponent(gameObjects.at(i)))
			{
				Component* comp = dynamic_cast<Component*>(gameObjects[i]);
				std::cout << "RED Detected Component!\nID: " << comp->getId() << "\n";
				currentPlayer->releaseItem();
				gameObjects[i]->setPos(DirectX::SimpleMath::Vector3(22.6783f, -21.7597f, 31.504f));
				spaceShipRed->addComponent();
				spaceShipRed->setAnimate(true);
				//return WIN;
			}

			//Check if BLU spaceship detected
			if (spaceShipBlue->detectedComponent(gameObjects.at(i)))
			{
				Component* comp = dynamic_cast<Component*>(gameObjects[i]);
				std::cout << "BLU Detected Component!\nID: " << comp->getId() << "\n";
				currentPlayer->releaseItem();
				gameObjects[i]->setPos(DirectX::SimpleMath::Vector3(22.6783f, -21.7597f, 31.504f));
				spaceShipBlue->setAnimate(true);
				spaceShipBlue->addComponent();
			}
		}
	}
	//Check winstate
	if (spaceShipBlue->isFinished())
	{
		if (currentPlayer->getTeam() == 1)
		{
			return WIN;
			std::cout << "You have repaired the ship and returned to earth\n";
		}
		else
		{
			return LOSE;
		}
	}	
	if (spaceShipRed->isFinished())
	{
		if (currentPlayer->getTeam() == 0)
		{
			return WIN;
			std::cout << "You have repaired the ship and returned to earth\n";
		}
		else
		{
			return LOSE;
		}
	}


	//Play pickup animation
	spaceShipBlue->animateOnPickup();
	spaceShipRed->animateOnPickup();


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
	GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
	atmosphere->draw();
	grenade->drawFresnel();

	//Render Skybox
	basicRenderer.skyboxPrePass();
	this->skybox.draw();
	basicRenderer.depthUnbind();


	//Render imgui & wireframe
	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (wireframe) { immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); playerVecRenderer.drawLines(); }

	//render billboard objects
	basicRenderer.bilboardPrePass(this->camera);
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

	//Render Particles
	basicRenderer.geometryPass(this->camera);
	drawParticles();
	basicRenderer.geometryUnbind();


	//Render UI (needs to render last)
	ui.Draw();
}

