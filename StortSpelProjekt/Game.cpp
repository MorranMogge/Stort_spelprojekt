#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	MaterialLibrary::LoadDefault();
	MaterialLibrary::LoadMaterial("spaceshipTexture1.jpg");
	MaterialLibrary::LoadMaterial("pintoRed.png");
	MaterialLibrary::LoadMaterial("pintoBlue.png");
	MaterialLibrary::LoadMaterial("Red.png");

	this->packetEventManager = new PacketEventManager();
	//m�ste raderas******************
	client = new Client();//("192.168.43.216");
	circularBuffer = client->getCircularBuffer();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);

	ltHandler.addLight(DirectX::XMFLOAT3(-90, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 1, 0),1);
	ltHandler.addLight(DirectX::XMFLOAT3(16 + 7, 42 + 17, 12 + 7), DirectX::XMFLOAT3(0, 0.3f, 1.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	ltHandler.addLight(DirectX::XMFLOAT3(-10 - 5, -45 - 17, -10 - 7), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	
	//ltHandler.addLight(DirectX::XMFLOAT3(10, -50, 30), DirectX::XMFLOAT3(0, 0, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);

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
		int dude = (NROFPLAYERS) / 2;

		for (int i = 0; i < NROFPLAYERS; i++)//initialize players 
		{
			Player* tmpPlayer = nullptr;
			
			if (playerId != i)
			{
				tmpPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(35.f + (float)(offset * i), 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, client, (int)(dude < i + 1), &planetGravityField);
				tmpPlayer->setOnlineID(i);
				physWolrd.addPhysComponent(tmpPlayer, reactphysics3d::CollisionShapeName::BOX);
				players.push_back(tmpPlayer);
			}
			else
			{
				currentPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(0, 42, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, client, (int)(dude < i + 1), &planetGravityField);
				currentPlayer->setOnlineID(i);
				players.push_back(currentPlayer);
				delete tmpPlayer;
			}
			std::cout << "Dude: " << (int)(dude < i + 1) << "\n";
		}
	}
	this->loadObjects();
	baseballBat->setGameObjects(players);
	baseballBat->setClient(client);

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
		//delete players[i];
	}
	for (int i = 0; i < planetVector.size(); i++)
	{
		delete planetVector[i];
	}
	delete asteroids;
	delete planetMeshes;
}

void Game::loadObjects()
{
	//Meshes vector contents
	//Sphere, pinto, potion, bat, Player, component, grenade

	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	float planetSize = 40.f;

	//Here we can add base object we want in the beginning of the game
	potion = new Potion("../Meshes/potion", DirectX::SimpleMath::Vector3(0, -40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 2, 0, &planetGravityField);
	baseballBat = new BaseballBat("../Meshes/bat", DirectX::SimpleMath::Vector3(40, 40, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 4, 0, &planetGravityField);
	grenade = new Grenade("../Meshes/grenade", DirectX::SimpleMath::Vector3(40, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 6, GRENADE, &planetGravityField);
	
	planetMeshes = new Mesh("../Meshes/Sphere");
	planetVector.emplace_back(new Planet(planetMeshes, DirectX::XMFLOAT3(planetSize, planetSize, planetSize)));
	planetVector.back()->setPlanetShape(&physWolrd);
	//for (int i = 1; i < 100; i++)
	//{
	//	planetVector.emplace_back(new Planet(planetMeshes, DirectX::XMFLOAT3(planetSize, planetSize, planetSize), DirectX::XMFLOAT3(rand()%100*i, rand() % 100 * i, rand() % 100)));
	//	planetVector.back()->setPlanetShape(&physWolrd);
	//}
	planetVector.emplace_back(new Planet(planetMeshes, DirectX::XMFLOAT3(planetSize, planetSize, planetSize), DirectX::XMFLOAT3(-55.f, -55.f, -55.f)));
	planetVector.back()->setPlanetShape(&physWolrd);
	asteroids = new AsteroidHandler(planetMeshes, physWolrd);
	//asteroids.emplace_back(new Asteroid(planetMeshes, physWolrd, DirectX::XMFLOAT3(100, 100, 100), DirectX::XMFLOAT3(-1, -1, -1), 0.05f));

	items.emplace_back(potion);
	items.emplace_back(baseballBat);
	items.emplace_back(grenade);

	for (int i = 0; i < items.size(); i++)
	{
		gameObjects.emplace_back(items[i]);
	}
	for (int i = 0; i < gameObjects.size(); i++)
	{
		physWolrd.addPhysComponent(gameObjects[i], reactphysics3d::CollisionShapeName::BOX);
		gameObjects[i]->getPhysComp()->setPosition(reactphysics3d::Vector3(gameObjects[i]->getPosV3().x, gameObjects[i]->getPosV3().y, gameObjects[i]->getPosV3().z));
	}

	baseballBat->setPlayer(currentPlayer);
	baseballBat->setTestObj(gameObjects);
	grenade->setGameObjects(gameObjects);
	
	if (!currentPlayer) { currentPlayer = new Player("../Meshes/pinto", DirectX::SimpleMath::Vector3(0, 42, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, client, 0, &planetGravityField); players.emplace_back(currentPlayer); }
	currentPlayer->setPhysComp(physWolrd.getPlayerBox());
	currentPlayer->getPhysComp()->setParent(currentPlayer);
	for (int i = 0; i < players.size(); i++)
	{
		gameObjects.emplace_back(players[i]);
	}
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
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
	}
	for (int i = 0; i < planetVector.size(); i++)
	{
		planetVector[i]->drawPlanet();
	}
	asteroids->drawAsteroids();

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

	randomPos.x = (float)xPos;
	randomPos.y = (float)yPos;
	randomPos.z = (float)zPos;

	randomPos.Normalize();
	randomPos *= 100;

	object->setPos(randomPos);
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

GAMESTATE Game::Update()
{
	//read the packets received from the server
	packetEventManager->PacketHandleEvents(circularBuffer, NROFPLAYERS, players, client->getPlayerId(), components, physWolrd, gameObjects, &planetGravityField, spaceShips, onlineItems);
	
	//Get newest delta time
	lastUpdate = currentTime;
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();

	if (asteroids->ifTimeToSpawnAsteroids()) asteroids->spawnAsteroids(planetVector[0]);
	asteroids->updateAsteroids(dt, planetVector, gameObjects);

	//Calculate gravity factor
	grav = planetVector[0]->getClosestFieldFactor(planetVector, currentPlayer->getPosV3());
	currentPlayer->updateVelocity(getScalarMultiplicationXMFLOAT3(dt, grav));
	//additionXMFLOAT3(velocity, getScalarMultiplicationXMFLOAT3(dt, grav));

	//Raycasting
	static DirectX::XMFLOAT3 hitPos;
	static DirectX::XMFLOAT3 hitNormal;
	hitPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	hitNormal = DirectX::XMFLOAT3(grav.x, grav.y, grav.z);
	bool testingVec = this->currentPlayer->raycast(gameObjects, planetVector, hitPos, hitNormal);
	if (testingVec || currentPlayer->getHitByBat()) currentPlayer->resetVelocity();
	
	//Player functions
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), hitNormal, dt, testingVec);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, gamePad, dt);
	currentPlayer->checkForStaticCollision(gameObjects);
	currentPlayer->checkMovement();
	currentPlayer->velocityMove(dt);

	for (int i = 0; i < components.size(); i++)
	{
		if (currentPlayer->pickupItem(components[i])) break;
	}

	for (int i = 0; i < items.size(); i++)
	{
		if (currentPlayer->pickupItem(items[i])) break;
	}

	grenade->updateExplosionCheck();
	if (potion->isTimeToRun())
	{
		if (potion->timerGoing()) currentPlayer->setSpeed(50.f);
		else currentPlayer->setSpeed(25.f);
	}

	//sending data to server
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
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
	if (noWinners) camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), currentPlayer->getUpVec());
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

	//Render Skybox
	basicRenderer.skyboxPrePass();
	this->skybox.draw();
	basicRenderer.depthUnbind();


	//Render imgui & wireframe
	imGui.react3D(wireframe, objectDraw, dt);
	if (wireframe) { physWolrd.renderReact3D(); playerVecRenderer.drawLines(); }

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
	//drawParticles();
	//this->ptEmitters.at(0).unbind();

	//drawParticles();	//not in use, intended for drawing particles in game.cpp
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
	basicRenderer.geometryUnbind();


	//Render UI (needs to render last)
	ui.Draw();
}

