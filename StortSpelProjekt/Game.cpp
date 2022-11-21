#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"
#include "MemoryLeackChecker.h"
#include "SoundCollection.h"

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0)), currentMinigame(MiniGames::COMPONENTCOLLECTION)
{
	this->packetEventManager = new PacketEventManager();
	gameMusic.load(L"../Sounds/Gold Rush Final.wav");
	gameMusic.play(true);
	gameMusic.setVolume(0.75f);
	//m�ste raderas******************
	client = new Client("192.168.43.251");
	circularBuffer = client->getCircularBuffer();

	//Setup rendering
	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);

	//Setup Lights
	ltHandler.addLight(DirectX::XMFLOAT3(-90, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 1, 0),1);
	ltHandler.addLight(DirectX::XMFLOAT3(16 + 7, 42 + 17, 12 + 7), DirectX::XMFLOAT3(0, 0.3f, 1.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	ltHandler.addLight(DirectX::XMFLOAT3(-10 - 5, -45 - 17, -10 - 7), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	

	//Load game objects
	this->loadObjects();

	//Setup players
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
		std::cout << "The recv player id from client: " << std::to_string(playerId) << std::endl;
		this->client->setClientId(playerId);
		int offset = 10;
		int dude = (NROFPLAYERS) / 2;

		for (int i = 0; i < NROFPLAYERS; i++)//initialize players 
		{
			Player* tmpPlayer = nullptr;
			
			if (playerId != i)
			{
				tmpPlayer = new Player(meshes[1], DirectX::SimpleMath::Vector3(35.f + (float)(offset * i), 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0, i, client, (int)(dude < i + 1), planetGravityField);
				tmpPlayer->setOnlineID(i);
				physWolrd.addPhysComponent(tmpPlayer, reactphysics3d::CollisionShapeName::BOX);
				players.push_back(tmpPlayer);
			}
			else
			{
				std::cout << "Player online id: " << std::to_string(i) << " \n";
				currentPlayer = new Player(meshes[1], DirectX::SimpleMath::Vector3(0, 42, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, playerId, client, (int)(dude < i + 1), planetGravityField);
				currentPlayer->setOnlineID(i);
				players.push_back(currentPlayer);
				delete tmpPlayer;
			}
			std::cout << "Dude: " << (int)(dude < i + 1) << "\n";
		}
	}
	currentPlayer->setPhysComp(physWolrd.getPlayerBox());
	currentPlayer->getPhysComp()->setParent(currentPlayer);
	gameObjects.emplace_back(currentPlayer);
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->setGravityField(planetGravityField);
	}

	field = planetVector[0]->getClosestField(planetVector, currentPlayer->getPosV3());
	oldField = field;

	//Set items baseball bat
	baseballBat->setPlayer(currentPlayer);
	baseballBat->setGameObjects(gameObjects);
	baseballBat->setClient(client);

	//Set items grenade
	grenade->setGameObjects(gameObjects);

	//Init delta time
	currentTime = std::chrono::system_clock::now();
	lastUpdate = currentTime;
	gamePad = std::make_unique<DirectX::GamePad>();
	playerVecRenderer.setPlayer(currentPlayer, camera);
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
	for (int i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	for (int i = 0; i < planetVector.size(); i++)
	{
		delete planetVector[i];
	}
	delete asteroids;
	delete arrow;
}

void Game::loadObjects()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;

	//Variables 
	SpaceShip* spaceShipRed;
	SpaceShip* spaceShipBlue;
	Component* component;


	//Load extra textures
	MaterialLibrary::LoadDefault();
	MaterialLibrary::LoadMaterial("spaceshipTexture1.jpg");
	MaterialLibrary::LoadMaterial("spaceshipTexture2.jpg");
	MaterialLibrary::LoadMaterial("pintoRed.png");
	MaterialLibrary::LoadMaterial("pintoBlue.png");
	MaterialLibrary::LoadMaterial("Red.png");

	//Meshes vector contents
	meshes.push_back(new Mesh("../Meshes/Sphere"));
	meshes.push_back(new Mesh("../Meshes/reverseSphere"));
	meshes.push_back(new Mesh("../Meshes/pinto"));
	meshes.push_back(new Mesh("../Meshes/potion"));
	meshes.push_back(new Mesh("../Meshes/rocket"));
	meshes.push_back(new Mesh("../Meshes/bat"));
	meshes.push_back(new Mesh("../Meshes/component"));
	meshes.push_back(new Mesh("../Meshes/grenade"));
	meshes.push_back(new Mesh("../Meshes/arrow"));

	//Planet::Planet( useMesh,scale, pos, gravityFactor, Mesh * atmoMesh, const DirectX::SimpleMath::Vector3 & atmoColor, const float& atmoDensity)

	//SOLAR SYSTEM SETUP
	float planetSize = 40.f;
	planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetSize, planetSize, planetSize), DirectX::XMFLOAT3(0,0,0),(4.0f * 9.82f),meshes[1]));
	planetVector.back()->setPlanetShape(&physWolrd);
	planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetSize, planetSize, planetSize), DirectX::XMFLOAT3(-55.f, -55.f, -55.f), (4.0f * 9.82f), meshes[1]));
	planetVector.back()->setPlanetShape(&physWolrd);
	asteroids = new AsteroidHandler(meshes[0]);
	planetGravityField = planetVector[0]->getGravityField();
	
	//Make sure the physics world has access to the planets
	physWolrd.setPlanets(planetVector);

	//CREATE ITEMS 	//Sphere, reverseSphere, pinto, potion, rocket, bat, component, grenade, arrow
	potion = new Potion(meshes[3], Vector3(0, 0, -42),Vector3(0.0f, 0.0f, 0.0f), POTION, 0, planetGravityField);
	baseballBat = new BaseballBat(meshes[5], Vector3(0, 0, 42), Vector3(0.0f, 0.0f, 0.0f), BAT, 0, planetGravityField);
	grenade = new Grenade(meshes[7], DirectX::SimpleMath::Vector3(42, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), GRENADE, 0, planetGravityField);
	arrow = new Arrow(meshes[8], DirectX::SimpleMath::Vector3(0, 42, 0));
	
	//currentPlayer = new Player(meshes[1], Vector3(0, 48, 0), Vector3(0.0f, 0.0f, 0.0f), PLAYER, client, 0, &planetGravityField);


	//EMPLACE ITEMS
	items.emplace_back(potion);
	items.emplace_back(baseballBat);
	items.emplace_back(grenade);

	for (int i = 0; i < items.size(); i++)
	{
		gameObjects.emplace_back(items[i]);
	}
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] != currentPlayer) gameObjects.emplace_back(players[i]);
	}

	
	//Add phys components
	for (int i = 0; i < gameObjects.size(); i++)
	{
		physWolrd.addPhysComponent(gameObjects[i], reactphysics3d::CollisionShapeName::BOX);
	}

	
	//SPACE SHIPS
	if (!IFONLINE)
	{
		spaceShipRed = new SpaceShip(meshes[4], Vector3(-7.81178f, -37.8586f, -8.50119f), ROCKET, 0, planetGravityField, DirectX::SimpleMath::Vector3(2, 2, 2));
		spaceShipBlue = new SpaceShip(meshes[4], Vector3(13.5817f, 35.9383f, 9.91351f), ROCKET, 1, planetGravityField, DirectX::SimpleMath::Vector3(2, 2, 2));

		spaceShips.emplace_back(spaceShipRed);
		spaceShips.emplace_back(spaceShipBlue);

		component = new Component(meshes[6], DirectX::SimpleMath::Vector3(0, -42, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), COMPONENT, 0, planetGravityField);
		components.emplace_back(component);
		gameObjects.emplace_back(component);
		physWolrd.addPhysComponent(component, reactphysics3d::CollisionShapeName::BOX);
	}
	for (int i = 0; i < spaceShips.size(); i++)
	{
		physWolrd.addPhysComponent(spaceShips[i], reactphysics3d::CollisionShapeName::BOX, DirectX::XMFLOAT3(0.75f, 4 * 0.75f, 0.75f));
		spaceShips[i]->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
		gameObjects.emplace_back(spaceShips[i]);
	}

	//Initilize player
	if (!currentPlayer && !IFONLINE) 
	{ 
		currentPlayer = new Player(meshes[2], DirectX::SimpleMath::Vector3(0, 48, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, client->getPlayerId(), client, 0, planetGravityField); 
		players.emplace_back(currentPlayer);
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
		if (gameObjects[i] == currentPlayer) continue;
		else gameObjects[i]->draw();
	}

	for (int i = 0; i < onlineItems.size(); i++)
	{
		onlineItems[i]->draw();
	}
	currentPlayer->updateBuffer();
	currentPlayer->draw();
	for (int i = 0; i < planetVector.size(); i++)
	{
		planetVector[i]->drawPlanet();
	}
	asteroids->drawAsteroids();

	//Draw with Ambient only shader
	basicRenderer.bindAmbientShader();
	arrow->draw();

	//Draw light debug meshes
	if (drawDebug)
	{
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
	for (int i = 0; i < onlineItems.size(); i++)
	{
		this->onlineItems[i]->drawIcon();
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
	//Regular
	for (int i = 0; i < spaceShips.size(); i++)
	{
		spaceShips[i]->drawFresnel();
	}
	for (int i = 0; i < items.size(); i++)
	{
		if(items[i]->getId() == ObjID::GRENADE)
		{
			Grenade* tempNade = (Grenade*)items[i];
			tempNade->drawFresnel();
		}
	}
	//Inverse
	basicRenderer.invFresnelPrePass();
	for (int i = 0; i < planetVector.size(); i++)
	{
		planetVector[i]->drawAtmosphere();
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

GAMESTATE Game::updateComponentGame()
{
	//Get newest delta time
	if (asteroids->ifTimeToSpawnAsteroids()) asteroids->spawnAsteroids(planetVector[0]);
	asteroids->updateAsteroids(dt, planetVector, gameObjects);

	//Calculate gravity factor
	field = planetVector[0]->getClosestField(planetVector, currentPlayer->getPosV3());
	if (field != oldField) { changedPlanet = true; currentPlayer->setGravityField(this->field); }
	else changedPlanet = false;
	oldField = field;

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
	currentPlayer->rotate(hitNormal, testingVec, changedPlanet);
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), dt);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), grav, gamePad, dt);
	currentPlayer->checkForStaticCollision(planetVector, spaceShips);
	currentPlayer->velocityMove(dt);

	//Check component pickup
	if (!IFONLINE)
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (currentPlayer->pickupItem(components[i])) break;
		}
	}
	//Check component pickup

	currentPlayer->requestingPickUpItem(onlineItems);

	//Check item pickup
	for (int i = 0; i < items.size(); i++)
	{
		if (currentPlayer->pickupItem(items[i])) break;
	}

	//grenade->updateExplosionCheck();
	//if (potion->isTimeToRun())
	//Update item checks
	for (int i = 0; i < items.size(); i++)
	{
		int id = items[i]->getId();
		switch (id)
		{
		case ObjID::GRENADE:
		{
			Grenade* tempNade = (Grenade*)items[i];
			tempNade->updateExplosionCheck();
			if (tempNade->getExploded() == true)
			{
				randomizeObjectPos(tempNade);
				tempNade->setExploded(false);
			}
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
	//currentPlayer->updateBuffer();

	//Updates gameObject physics components
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}

	//Setting the camera at position
	if (!velocityCamera) camera.moveCamera(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), currentPlayer->getUpVector(), currentPlayer->getSpeed(), dt);
	else camera.moveVelocity(currentPlayer->getPosV3(), currentPlayer->getRotationMX(), currentPlayer->getUpVector(), currentPlayer->getSpeed(), dt);

	this->arrow->moveWithCamera(currentPlayer->getPosV3(), DirectX::XMVector3Normalize(camera.getForwardVector()), currentPlayer->getUpVector(), currentPlayer->getRotationMX());

	//Check Components online
	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (spaceShips[i]->getCompletion())
		{
			if (currentPlayer->getTeam() == i) camera.winScene(spaceShips[i]->getPosV3(), spaceShips[i]->getRot());
			this->spaceShips[i]->move(this->spaceShips[i]->getUpDirection(), -dt);
			endTimer += dt;
			arrow->removeArrow(); //Remove these completely by not drawing the meshes anymore
			if (currentPlayer->getTeam() == i) this->currentPlayer->setPos(DirectX::XMFLOAT3(6969, 6969, 6969)); //Remove these completely by not drawing the meshes anymore
		}
	}

	if (components.size() > 0)
	{
		//Arrow pointing to spaceship
		if (currentPlayer->isHoldingComp())
		{
			for (int i = 0; i < spaceShips.size(); i++)
			{
				if (currentPlayer->getTeam() == i) this->arrow->showDirection(spaceShips[i]->getPosV3(), currentPlayer->getPosV3(), planetGravityField->calcGravFactor(arrow->getPosition()));
			}
		}
		//Arrow pointing to component
		else this->arrow->showDirection(components[0]->getPosV3(), currentPlayer->getPosV3(), grav);
	}

	if (!IFONLINE) //Check Components offline
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			for (int j = 0; j < components.size(); j++)
			{
				if (spaceShips[i]->detectedComponent(components[j]))
				{
					if (currentPlayer->getItem() != nullptr)
					{
						if (currentPlayer->getItem()->getId() == ObjID::COMPONENT)
						{
							currentPlayer->releaseItem();
						}
					}
					randomizeObjectPos(components[j]);
					spaceShips[i]->addComponent();
					spaceShips[i]->setAnimate(true);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			for (int j = 0; j < components.size(); j++)
			{
				spaceShips[i]->detectedComponent(components[j]);
			}
		}
	}
	//Check winstate
	if (endTimer > 6)
	{
		if (endTimer > 10) currentMinigame = MiniGames::LANDINGSPACESHIP;

		//Fade into black


		/*for (int i = 0; i < spaceShips.size(); i++)
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
		}*/
	}

	//Play pickup animation
	for (int i = 0; i < spaceShips.size(); i++)
	{
		spaceShips[i]->animateOnPickup();
	}

	//Check if item icon should change to pickup icon 
	for (int i = 0; i < items.size(); i++)
	{
		this->items[i]->checkDistance((GameObject*)(currentPlayer));
	}
	for (int i = 0; i < components.size(); i++)
	{
		this->components[i]->checkDistance((GameObject*)(currentPlayer));
	}

	return NOCHANGE;
}

GAMESTATE Game::updateLandingGame()
{
	//Here yo type the function below but replace testObject with your space ship
	camera.landingMinigameScene(planetVector[0], spaceShips[currentPlayer->getTeam()]->getPosV3(), spaceShips[currentPlayer->getTeam()]->getRot());
	DirectX::SimpleMath::Vector3 moveDir = getScalarMultiplicationXMFLOAT3(1, (planetVector[0]->getGravityField()->calcGravFactor(spaceShips[currentPlayer->getTeam()]->getPos())));

	moveDir.Normalize();
	moveDir *= 0.5f;
	spaceShips[currentPlayer->getTeam()]->move(moveDir, dt);
	if (landingUi.handleInputs(dt)) landingMiniGamePoints += 100*dt;
	if (getLength(spaceShips[currentPlayer->getTeam()]->getPosV3()) <= planetVector[0]->getSize())
	{
		moveDir.Normalize();
		spaceShips[currentPlayer->getTeam()]->setPos(moveDir * planetVector[0]->getSize());
		currentMinigame = MiniGames::COMPONENTCOLLECTION;
		std::cout << "Total points " << landingMiniGamePoints << std::endl;

		//Send data to server
		LandingMiniGameOver totalPoints;
		totalPoints.packetId = PacketType::LANDINGMINIGAMEOVER;
		totalPoints.points = landingMiniGamePoints;

		client->sendStuff<LandingMiniGameOver>(totalPoints);
	}

	return NOCHANGE;
}

GAMESTATE Game::updateKingOfTheHillGame()
{
	return NOCHANGE;

}

GAMESTATE Game::Update()
{
	//read the packets received from the server
	packetEventManager->PacketHandleEvents(circularBuffer, NROFPLAYERS, players, client->getPlayerId(), components, physWolrd, gameObjects, planetGravityField, spaceShips, onlineItems, meshes);
	
	lastUpdate = currentTime;
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();

	if (Input::KeyPress(KeyCode::P)) 
	{ 
		currentMinigame = MiniGames::LANDINGSPACESHIP;
		DirectX::XMFLOAT3 newRot = spaceShips[currentPlayer->getTeam()]->getUpDirection();
		spaceShips[currentPlayer->getTeam()]->setPos(newRot * DirectX::SimpleMath::Vector3(150, 150, 150));
	}

	//Simulate the current minigame on client side
	switch (currentMinigame)
	{
	case COMPONENTCOLLECTION:
		currentGameState = this->updateComponentGame();
		break;
	case LANDINGSPACESHIP:
		currentGameState = this->updateLandingGame();
		break;
	case KINGOFTHEHILL:
		currentGameState = this->updateKingOfTheHillGame();
		break;
	default:
		break;
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


	//Render Skybox
	basicRenderer.skyboxPrePass();
	skybox.draw();
	basicRenderer.depthUnbind();

	//Render imgui & wireframe
	imGui.react3D(wireframe, objectDraw, landingMinigame, dt, velocityCamera);
	if (wireframe) { physWolrd.renderReact3D(); playerVecRenderer.drawLines(); }

	//render billboard objects
	basicRenderer.bilboardPrePass(this->camera);
	drawIcons();

	//Render Particles
	basicRenderer.geometryPass(this->camera);
	drawParticles();
	basicRenderer.geometryUnbind();

	//Render UI (needs to render last)
	ui.Draw();
	switch (currentMinigame)
	{
	case COMPONENTCOLLECTION:
		break;
	case LANDINGSPACESHIP:
		landingUi.draw();
		break;
	case KINGOFTHEHILL:
		break;
	default:
		break;
	}
}

