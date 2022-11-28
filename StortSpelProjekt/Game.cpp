#include "stdafx.h"
#include "Game.h"
#include "DirectXMathHelper.h"
#include "SendingDataEvent.h"
#include "MemoryLeackChecker.h"
#include "SoundCollection.h"

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window, UINT WIDTH, UINT HEIGHT)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0)), manager(ModelManager(device)), currentMinigame(MiniGames::COMPONENTCOLLECTION)
{
	srand(time(0));

	this->HEIGHT = HEIGHT;
	this->WIDTH = WIDTH;
	this->packetEventManager = new PacketEventManager();
	gameMusic.load(L"../Sounds/Gold Rush Final.wav");
	gameMusic.play(true);
	gameMusic.setVolume(0.75f);
	//m�ste raderas******************
	client = new Client();
	std::cout << "Game is setup for " << std::to_string(NROFPLAYERS) << std::endl;
	circularBuffer = client->getCircularBuffer();

	//Setup rendering
	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);

	//Setup Lights
	ltHandler.addLight(DirectX::XMFLOAT3(-90, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 1);
	ltHandler.addLight(DirectX::XMFLOAT3(16 + 7, 42 + 17, 12 + 7), DirectX::XMFLOAT3(0, 0.3f, 1.0f), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);
	ltHandler.addLight(DirectX::XMFLOAT3(-10 - 5, -45 - 17, -10 - 7), DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0), 2);

	manager.loadMeshAndBoneData("../Meshes/pinto_Run.fbx");
	this->manager.getAnimData("../Meshes/pinto_Run.fbx", vBuff, iBuff, subMeshRanges, verticies, animData);
	ID3D11ShaderResourceView* blueTeamColour = this->manager.getSrv("../Textures/pintoBlue.png");
	ID3D11ShaderResourceView* redTeamColour = this->manager.getSrv("../Textures/pintoRed.png");
	this->tmpMesh = new Mesh(vBuff, iBuff, subMeshRanges, verticies);

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
				tmpPlayer = new Player(tmpMesh, DirectX::SimpleMath::Vector3(35.f + (float)(offset * i), 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 
					0, i, client, (int)(dude < i + 1), redTeamColour, blueTeamColour, planetGravityField);
				tmpPlayer->addData(animData);
				tmpPlayer->setOnlineID(i);
				physWorld.addPhysComponent(tmpPlayer, reactphysics3d::CollisionShapeName::BOX);
				players.push_back(tmpPlayer);
			}
			else
			{
				std::cout << "Player online id: " << std::to_string(i) << " \n";
				currentPlayer = new Player(tmpMesh, DirectX::SimpleMath::Vector3(0, 42, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
					1, playerId, client, (int)(dude < i + 1), redTeamColour, blueTeamColour, planetGravityField);
				currentPlayer->addData(animData);
				currentPlayer->setOnlineID(i);
				players.push_back(currentPlayer);
				delete tmpPlayer;
			}
			std::cout << "Dude: " << (int)(dude < i + 1) << "\n";
		}

		gamePad = new DirectX::GamePad();
		currentPlayer->setGamePad(gamePad);
	}

	currentPlayer->setPhysComp(physWorld.getPlayerBox());
	currentPlayer->getPhysComp()->setParent(currentPlayer);
	gameObjects.emplace_back(currentPlayer);
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->setGravityField(planetGravityField);
	}

	field = nullptr;
	oldField = field;

	//Set items baseball bat
	if (!IFONLINE)
	{
		baseballBat->setPlayer(currentPlayer);
		baseballBat->setGameObjects(gameObjects);

		//Set items grenade
		grenade->setGameObjects(gameObjects);
	}
	//baseballBat->setClient(client);

	

	//Init delta time
	currentTime = std::chrono::system_clock::now();
	lastUpdate = currentTime;
	//gamePad = std::make_unique<DirectX::GamePad>();
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
	if (captureZone != nullptr) delete captureZone;
	delete tmpMesh;
	delete asteroids;
	delete arrow;
	delete planetGravityField;
	delete gamePad;
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
	meshes.push_back(new Mesh("../Meshes/zone"));

	//Planet::Planet( useMesh,scale, pos, gravityFactor, Mesh * atmoMesh, const DirectX::SimpleMath::Vector3 & atmoColor, const float& atmoDensity)

	//SOLAR SYSTEM SETUP
	if (!IFONLINE)
	{
		float planetSize = 40.f; // SET DIFFERENT GRAV-FACTORS FOR THE PLANETS AND DIFFERENT TEXTURES!
		planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetSize, planetSize, planetSize), DirectX::XMFLOAT3(0.f, 0.f, 0.f)));
		planetVector.back()->setPlanetShape(&physWorld);
		planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetSize * 0.8f, planetSize * 0.8f, planetSize * 0.8f), DirectX::XMFLOAT3(60.f, 60.f, 60.f)));
		planetVector.back()->setPlanetShape(&physWorld);
		planetVector.emplace_back(new Planet(meshes[0], DirectX::XMFLOAT3(planetSize * 0.6f, planetSize * 0.6f, planetSize * 0.6f), DirectX::XMFLOAT3(-130.f, -130.f, 130.f)));
		planetVector.back()->setPlanetShape(&physWorld);
		physWorld.setPlanets(planetVector);
	}

	asteroids = new AsteroidHandler(meshes[0]);
	planetGravityField = new GravityField(4.f * 9.82f, DirectX::XMFLOAT3(0.f, 0.f, 0.f), 40.f);

	//Make sure the physics world has access to the planets

	//CREATE ITEMS 	//Sphere, reverseSphere, pinto, potion, rocket, bat, component, grenade, arrow
	if (!IFONLINE)
	{
		potion = new Potion(meshes[3], Vector3(0, 0, -42), Vector3(0.0f, 0.0f, 0.0f), POTION, 0, planetGravityField);
		baseballBat = new BaseballBat(meshes[5], Vector3(0, 0, 42), Vector3(0.0f, 0.0f, 0.0f), BAT, 0, planetGravityField);
		grenade = new Grenade(meshes[7], DirectX::SimpleMath::Vector3(42, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), GRENADE, 0, planetGravityField);

		//EMPLACE ITEMS
		items.emplace_back(potion);
		items.emplace_back(baseballBat);
		items.emplace_back(grenade);
	}
	arrow = new Arrow(meshes[8], DirectX::SimpleMath::Vector3(0, 42, 0));

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
		physWorld.addPhysComponent(gameObjects[i], reactphysics3d::CollisionShapeName::BOX);
	}

	//SPACE SHIPS
	if (!IFONLINE)
	{
		spaceShipRed = new SpaceShip(meshes[4], Vector3(-7.81178f, -37.8586f, -8.50119f), ROCKET, 0, planetGravityField, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2));
		spaceShipBlue = new SpaceShip(meshes[4], Vector3(13.5817f, 35.9383f, 9.91351f), ROCKET, 1, planetGravityField, meshes[9], DirectX::SimpleMath::Vector3(2, 2, 2));

		spaceShips.emplace_back(spaceShipRed);
		spaceShips.emplace_back(spaceShipBlue);

		component = new Component(meshes[6], DirectX::SimpleMath::Vector3(0, -42, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), COMPONENT, 0, planetGravityField);
		components.emplace_back(component);
		gameObjects.emplace_back(component);
		physWorld.addPhysComponent(component, reactphysics3d::CollisionShapeName::BOX);
	}
	for (int i = 0; i < spaceShips.size(); i++)
	{
		physWorld.addPhysComponent(spaceShips[i], reactphysics3d::CollisionShapeName::BOX, DirectX::XMFLOAT3(0.75f, 4 * 0.75f, 0.75f));
		spaceShips[i]->getPhysComp()->setType(reactphysics3d::BodyType::STATIC);
		gameObjects.emplace_back(spaceShips[i]);
	}

	//Initilize player
	if (!currentPlayer && !IFONLINE)
	{
		ID3D11ShaderResourceView* blueTeamColour = this->manager.getSrv("../Textures/pintoBlue.png");
		ID3D11ShaderResourceView* redTeamColour = this->manager.getSrv("../Textures/pintoRed.png");

		currentPlayer = new Player(tmpMesh, DirectX::SimpleMath::Vector3(0, 48, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),
			1, client->getPlayerId(), client, 0, redTeamColour, blueTeamColour, planetGravityField);
		currentPlayer->addData(animData);
		players.emplace_back(currentPlayer);
		gamePad = new GamePad();
		currentPlayer->setGamePad(gamePad);
	}

	if (!IFONLINE) captureZone = new CaptureZone(meshes[9], DirectX::SimpleMath::Vector3(42, 0, 0), DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f), planetGravityField, DirectX::SimpleMath::Vector3(10.f, 10.f, 10.f));
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
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->draw();
	}
	for (int i = 0; i < onlineItems.size(); i++)
	{
		onlineItems[i]->draw();
	}
	//currentPlayer->updateBuffer();
	currentPlayer->draw();
	for (int i = 0; i < planetVector.size(); i++)
	{
		if (i == camera.getCollidedWith()) continue;
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
	if (captureZone != nullptr) captureZone->drawFresnel();
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->getId() == ObjID::GRENADE)
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
	if (GetAsyncKeyState('C')) physWorld.addBoxToWorld();
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
	//if (asteroids->ifTimeToSpawnAsteroids()) asteroids->spawnAsteroids(planetVector[0]);
	//asteroids->updateAsteroids(dt, planetVector, gameObjects);

	//Set moon
	if (planetVector.size() > 2) planetVector[2]->rotateMoon(DirectX::XMFLOAT3(0, 0, 0), 0.5f);
	
	//Calculate gravity factor
	if (planetVector.size() > 0) field = planetVector[0]->getClosestField(planetVector, currentPlayer->getPosV3());
	if (field != oldField) { changedPlanet = true; currentPlayer->setGravityField(this->field); }
	else changedPlanet = false;
	oldField = field;

	if (planetVector.size() > 0) grav = field->calcGravFactor(currentPlayer->getPosV3());
	currentPlayer->updateVelocity(getScalarMultiplicationXMFLOAT3(dt, grav));

	if (planetVector.size() > 0)
	{
		for (int i = 0; i < gameObjects.size(); i++) gameObjects[i]->setGravityField(planetVector[0]->getClosestField(planetVector, gameObjects[i]->getPosV3()));
	}

	currentPlayer->velocityMove(dt);
	//Raycasting
	static DirectX::XMFLOAT3 hitPos;
	static DirectX::XMFLOAT3 hitNormal;
	hitPos = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	hitNormal = DirectX::XMFLOAT3(grav.x, grav.y, grav.z);
	bool testingVec;
	testingVec = this->currentPlayer->raycast(gameObjects, planetVector, hitPos, hitNormal);
	if (testingVec || currentPlayer->getHitByBat()) currentPlayer->resetVelocity();

	//Player functions
	currentPlayer->rotate(hitNormal, testingVec, changedPlanet);
	currentPlayer->move(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), dt);
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), dt);
	currentPlayer->checkForStaticCollision(planetVector, spaceShips);
	currentPlayer->setSpeed(20.f);

	//Check component pickup
	if (!IFONLINE) currentPlayer->pickupItem(items, components);
	currentPlayer->requestingPickUpItem(onlineItems);

	//Update item checks
	for (int i = 0; i < items.size(); i++)
	{
		id = items[i]->getId();
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
		} break;

		case ObjID::POTION:
		{
			Potion* tempPotion = (Potion*)items[i];
			if (tempPotion->timerGoing())
			{
				currentPlayer->setSpeed(50.f);
			}
		}	break;
		}
	}

	/*if (Input::KeyPress(KeyCode::K))
	{
		randomizeObjectPos(this->testBat);
	}*/

	//sending data to server
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		SendingDataEvent(client, currentPlayer, players);
		serverStart = std::chrono::system_clock::now();
	}

	//Physics related functions
	if (!IFONLINE) physWorld.update(dt);
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->updateMatrixOnline();
		players[i]->update();
	}

	//Updates gameObject physics components
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}

	//Setting the camera at position
	if (!velocityCamera) camera.moveVelocity(currentPlayer, dt);
	else camera.collisionCamera(currentPlayer, planetVector, dt);
	arrow->moveWithCamera(currentPlayer->getPosV3(), DirectX::XMVector3Normalize(camera.getForwardVector()), currentPlayer->getUpVector(), currentPlayer->getRotationMX());

	//Check Components online
	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (spaceShips[i]->getCompletion())
		{
			if (currentPlayer->getTeam() == i) camera.winScene(spaceShips[i]->getPosV3(), spaceShips[i]->getRot()); currentPlayer->setVibration(0.1f, 0.1f);
			this->spaceShips[i]->flyAway(dt);
			endTimer += dt;
			arrow->removeArrow(); //Remove these completely by not drawing the meshes anymore
			if (currentPlayer->getTeam() == i) this->currentPlayer->setPos(DirectX::XMFLOAT3(6969, 6969, 6969)); //Remove these completely by not drawing the meshes anymore
		}
	}

	//Arrow pointing to spaceship
	if (currentPlayer->isHoldingComp())
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			if (currentPlayer->getTeam() == i) this->arrow->showDirection(spaceShips[i]->getPosV3(), currentPlayer->getPosV3(), planetGravityField->calcGravFactor(arrow->getPosition()));
		}
	}
	//Arrow pointing to component
	else if (onlineItems.size() > 0)  this->arrow->showDirection(onlineItems[0]->getPosV3(), currentPlayer->getPosV3(), grav);
	else if (components.size() > 0) this->arrow->showDirection(components[0]->getPosV3(), currentPlayer->getPosV3(), grav);
	currentPlayer->colliedWIthComponent(components);


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
			for (int j = 0; j < onlineItems.size(); j++)
			{
				spaceShips[i]->detectedComponent(onlineItems[j]);
			}
		}
	}
	//Check winstate
	if (endTimer > 6)
	{
		this->currentPlayer->setVibration(0.f, 0.f);
		for (int i = 0; i < spaceShips.size(); i++)
		{
			if (spaceShips[i]->isFinished())
			{
				//Send data to server
				/*IntermissionStart startOfIntermission;
				startOfIntermission.packetId = PacketType::STARTINTERMISSION;
				client->sendStuff<IntermissionStart>(startOfIntermission);*/

				/*int team = spaceShips[i]->getTeam();
				if (currentPlayer->getTeam() == team)
				{
					return WIN;
				}
				else
				{
					return LOSE;
				}*/
			}
		}
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

GAMESTATE Game::startLanding()
{
	currentMinigame = LANDINGSPACESHIP;
	currentPlayer->setPos(DirectX::SimpleMath::Vector3(0.f, 62.f, 0.f));

	int index = currentPlayer->getTeam() + 1;
	int team = currentPlayer->getTeam();

	for (int i = 0; i < spaceShips.size(); i++)
	{
		if (i == currentPlayer->getTeam()) { index = currentPlayer->getTeam() + 1; team = currentPlayer->getTeam(); }
		else { index = (int)!currentPlayer->getTeam() + 1; team = (int)!currentPlayer->getTeam(); }
		DirectX::SimpleMath::Vector3 planetOffset = planetVector[index]->getPlanetPosition();
		planetOffset.Normalize();
		planetOffset *= 150.f;
		DirectX::SimpleMath::Vector3 newPos = planetVector[index]->getPlanetPosition() + planetOffset;
		spaceShips[team]->setPos(newPos);
		spaceShips[team]->setGravityField(planetVector[index]->getGravityField());
		spaceShips[team]->setRot(spaceShips[team]->getRotOrientedToGrav());
	}

	landingMiniGamePoints = 0.f;
	return GAMESTATE::NOCHANGE;
}

GAMESTATE Game::updateLandingGame()
{
	//Here yo type the function below but replace testObject with your space ship
	camera.landingMinigameScene(planetVector[currentPlayer->getTeam() + 1], spaceShips[currentPlayer->getTeam()]->getPosV3(), spaceShips[currentPlayer->getTeam()]->getRot());
	
	//Moves the spaceShips
	spaceShips[currentPlayer->getTeam()]->fly(spaceShips[currentPlayer->getTeam()]->getUpDirection(), dt);
	spaceShips[!currentPlayer->getTeam()]->fly(spaceShips[!currentPlayer->getTeam()]->getUpDirection(), dt);

	if (landingUi.handleInputs(dt)) landingMiniGamePoints += 100*dt;
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		std::cout << "Team score: " << teamScoreLandingMiniGame << "\nEnemy Team score: " << enemyTeamScoreLandingMiniGame << "\n";

		//Send data to server
		LandingMiniSendScoreToServer totalPoints = {};
		totalPoints.packetId = PacketType::LANDINGMINIGAMESENDSCORETOSERVER;
		totalPoints.playerId = currentPlayer->getOnlineID();
		totalPoints.scoreToServer = landingMiniGamePoints;
		client->sendStuff<LandingMiniSendScoreToServer>(totalPoints);
		serverStart = std::chrono::system_clock::now();
	}

	DirectX::SimpleMath::Vector3 vecToPlanet = spaceShips[currentPlayer->getTeam()]->getPosV3() - planetVector[currentPlayer->getTeam() + 1]->getPlanetPosition();

	if (getLength(vecToPlanet) <= planetVector[currentPlayer->getTeam() + 1]->getSize())
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			DirectX::SimpleMath::Quaternion dx11Quaternion = DirectX::XMQuaternionRotationMatrix(spaceShips[i]->getRot());
			reactphysics3d::Quaternion reactQuaternion = reactphysics3d::Quaternion(dx11Quaternion.x, dx11Quaternion.y, dx11Quaternion.z, dx11Quaternion.w);
			spaceShips[i]->getPhysComp()->setPosition(reactphysics3d::Vector3(spaceShips[i]->getPosV3().x, spaceShips[i]->getPosV3().y, spaceShips[i]->getPosV3().z));
			spaceShips[i]->getPhysComp()->setRotation(reactQuaternion);
		}
		std::cout << "\nLANDING MINIGAME OVER!\nTOTAL SCORE:\nTeam score: " << teamScoreLandingMiniGame << "\nEnemy Team score: " << enemyTeamScoreLandingMiniGame << "\n";


		//Send data to server
		DoneWithGame requestStart;
		requestStart.packetId = PacketType::DONEWITHGAME;
		requestStart.playerID = currentPlayer->getOnlineID();
		requestStart.formerGame = MiniGames::LANDINGSPACESHIP;
		client->sendStuff<DoneWithGame>(requestStart);
		currentMinigame = MiniGames::DEFAULT;
	}
	return NOCHANGE;
}

GAMESTATE Game::updateKingOfTheHillGame()
{
	//Get newest delta time
	//if (asteroids->ifTimeToSpawnAsteroids()) asteroids->spawnAsteroids(planetVector[0]);
	//asteroids->updateAsteroids(dt, planetVector, gameObjects);

	//Calculate gravity factor
	if (planetVector.size() > 0) field = planetVector[0]->getClosestField(planetVector, currentPlayer->getPosV3());
	if (field != oldField) { changedPlanet = true; currentPlayer->setGravityField(this->field); }
	else changedPlanet = false;
	oldField = field;
	if (planetVector.size() > 0) grav = field->calcGravFactor(currentPlayer->getPosV3());
	currentPlayer->updateVelocity(getScalarMultiplicationXMFLOAT3(dt, grav));

	if (planetVector.size() > 0)
	{
		for (int i = 0; i < gameObjects.size(); i++) gameObjects[i]->setGravityField(planetVector[0]->getClosestField(planetVector, gameObjects[i]->getPosV3()));
	}

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
	currentPlayer->moveController(DirectX::XMVector3Normalize(camera.getForwardVector()), DirectX::XMVector3Normalize(camera.getRightVector()), dt);
	currentPlayer->checkForStaticCollision(planetVector, spaceShips);
	currentPlayer->velocityMove(dt);
	currentPlayer->setSpeed(20.f);

	//Check component pickup
	currentPlayer->requestingPickUpItem(onlineItems);

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
			if (tempPotion->timerGoing()) currentPlayer->setSpeed(50.f);
		}	break;
		}
		break;
	}

	//sending data to server
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		SendingDataEvent(client, currentPlayer, players);
		serverStart = std::chrono::system_clock::now();
	}

	//Physics related functions
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->updateMatrixOnline();
		players[i]->update();
	}

	//Updates gameObject physics components
	for (int i = 0; i < gameObjects.size(); i++) gameObjects[i]->update();

	//Setting the camera at position
	if (!velocityCamera) camera.moveVelocity(currentPlayer, dt);
	else camera.collisionCamera(currentPlayer, planetVector, dt);
	arrow->moveWithCamera(currentPlayer->getPosV3(), DirectX::XMVector3Normalize(camera.getForwardVector()), currentPlayer->getUpVector(), currentPlayer->getRotationMX());

	//Arrow pointing to spaceship		FIX!
	if (currentPlayer->isHoldingComp())
	{
		for (int i = 0; i < spaceShips.size(); i++)
		{
			if (currentPlayer->getTeam() == i) this->arrow->showDirection(spaceShips[i]->getPosV3(), currentPlayer->getPosV3(), planetGravityField->calcGravFactor(arrow->getPosition()));
		}
	}
	//Arrow pointing to component
	if (captureZone) this->arrow->showDirection(captureZone->getPosition(), currentPlayer->getPosV3(), grav);
	currentPlayer->colliedWIthComponent(components);

	//Play pickup animation
	for (int i = 0; i < spaceShips.size(); i++) spaceShips[i]->animateOnPickup();

	//Check if item icon should change to pickup icon 
	for (int i = 0; i < items.size(); i++) this->items[i]->checkDistance((GameObject*)(currentPlayer));
	for (int i = 0; i < components.size(); i++) this->components[i]->checkDistance((GameObject*)(currentPlayer));
	return NOCHANGE;
}

GAMESTATE Game::startIntermission()
{
	this->camera.setPosition(DirectX::XMFLOAT3(100.f, 0.f, 300.f));
	this->camera.setRotToStart();
	this->camera.setCameraLookAt(DirectX::XMFLOAT3(150.f, 0, 300.f));

	this->spaceShips[0]->setRot(DirectX::XMFLOAT3(-DirectX::XM_PI * 0.5, 0, 0));
	this->spaceShips[1]->setRot(DirectX::XMFLOAT3(-DirectX::XM_PI * 0.5, 0, 0));

	this->centerPos = DirectX::XMFLOAT3(150, 0, 400);
	this->offset = DirectX::XMFLOAT2(15, 7);
	//this->spaceShips[0]->setPos(DirectX::XMFLOAT3(150, 7, 95));
	//this->spaceShips[1]->setPos(DirectX::XMFLOAT3(150, -7, 290));
	this->Stage = 0;

	currentMinigame = INTERMISSION;
	return NOCHANGE;
}

GAMESTATE Game::updateIntermission()
{
	float camSpeed = 10 * dt;
	totalTime += dt;
	DirectX::XMFLOAT3 camPos;
	DirectX::XMStoreFloat3(&camPos, camera.getRealPosition());
	camPos.z -= camSpeed;
	//DirectX::XMFLOAT3 midPos = camPos;
	//midPos.z += -20;
	this->camera.setPosition(camPos);
	camPos.x += 50;
	this->camera.setCameraLookAt(camPos);

	if (this->Stage == 0)
	{
		if (this->centerPos.z - camSpeed <= camPos.z)
		{
			this->Stage = 1;
			this->timer = 0;
		}
		else
		{
			camSpeed *= 2;
		}
	}
	else if (this->Stage == 1)
	{
		this->timer += dt;
		if (timer >= 5.0f)
		{
			this->Stage = 3;
		}
	}
	else if (Stage == 3)
	{
		camSpeed *= 2;
		if (this->centerPos.z + 80 <= camPos.z)
		{
			this->spaceShips[0]->setRot(spaceShips[0]->getRotOrientedToGrav());
			this->spaceShips[0]->getPhysComp()->setRotation(reactphysics3d::Quaternion(spaceShips[0]->getRotXM().x, spaceShips[0]->getRotXM().y, spaceShips[0]->getRotXM().z, 1.0));
			this->spaceShips[1]->setRot(spaceShips[1]->getRotOrientedToGrav());
			this->spaceShips[1]->getPhysComp()->setRotation(reactphysics3d::Quaternion(spaceShips[1]->getRotXM().x, spaceShips[1]->getRotXM().y, spaceShips[1]->getRotXM().z, 1.0));

			//Send data to server
			DoneWithGame requestStart;
			requestStart.packetId = PacketType::DONEWITHGAME;
			requestStart.playerID = currentPlayer->getOnlineID();
			requestStart.formerGame = MiniGames::INTERMISSION;
			client->sendStuff<DoneWithGame>(requestStart);
			currentMinigame = MiniGames::DEFAULT;
			return GAMESTATE::NOCHANGE;
		}
	}

	this->centerPos.z -= camSpeed;
	DirectX::XMFLOAT3 spacePos = centerPos;
	spacePos.z += offset.x;
	spacePos.y += offset.y;
	spacePos.y += sin(totalTime + offset.x);
	this->spaceShips[0]->setPos(spacePos);

	spacePos = centerPos;
	spacePos.z -= offset.x;
	spacePos.y -= offset.y;
	spacePos.y += sin(totalTime);
	this->spaceShips[1]->setPos(spacePos);
	return NOCHANGE;
}

GAMESTATE Game::Update()
{
	//read the packets received from the server
	packetEventManager->PacketHandleEvents(circularBuffer, NROFPLAYERS, players, client->getPlayerId(), components, physWorld, gameObjects, planetGravityField, spaceShips, onlineItems, meshes, planetVector, captureZone, currentMinigame,
		teamScoreLandingMiniGame, enemyTeamScoreLandingMiniGame, client);

	lastUpdate = currentTime;
	currentTime = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(currentTime - lastUpdate)).count();

	if (Input::KeyPress(KeyCode::P)) //THIS WILL BE REMOVED WHEN SWITCHING BETWEEN STATES IS DONE
	{ 
		currentMinigame = MiniGames::LANDINGSPACESHIP;
		
		int index = currentPlayer->getTeam() + 1;
		int team = currentPlayer->getTeam();

		for (int i = 0; i < spaceShips.size(); i++)
		{
			if (i == currentPlayer->getTeam()) { index = currentPlayer->getTeam() + 1; team = currentPlayer->getTeam(); }
			else { index = (int)!currentPlayer->getTeam() + 1; team = (int)!currentPlayer->getTeam(); }
			DirectX::SimpleMath::Vector3 planetOffset = planetVector[index]->getPlanetPosition();
			planetOffset.Normalize();
			planetOffset *= 150.f;
			DirectX::SimpleMath::Vector3 newPos = planetVector[index]->getPlanetPosition() + planetOffset;
			spaceShips[team]->setPos(newPos);
			spaceShips[team]->setGravityField(planetVector[index]->getGravityField());
			spaceShips[team]->setRot(spaceShips[team]->getRotOrientedToGrav());
		}

		landingMiniGamePoints = 0.f;
	}

	if (Input::KeyPress(KeyCode::L))
	{
		currentMinigame = MiniGames::INTERMISSION;
		this->camera.setPosition(DirectX::XMFLOAT3(100.f, 0.f, 300.f));
		this->camera.setRotToStart();
		this->camera.setCameraLookAt(DirectX::XMFLOAT3(150.f, 0, 300.f));

		this->spaceShips[0]->setRot(DirectX::XMFLOAT3(-DirectX::XM_PI * 0.5, 0, 0));
		this->spaceShips[1]->setRot(DirectX::XMFLOAT3(-DirectX::XM_PI * 0.5, 0, 0));

		this->centerPos = DirectX::XMFLOAT3(150, 0, 400);
		this->offset = DirectX::XMFLOAT2(15, 7);
		//this->spaceShips[0]->setPos(DirectX::XMFLOAT3(150, 7, 95));
		//this->spaceShips[1]->setPos(DirectX::XMFLOAT3(150, -7, 290));
		this->Stage = 0;
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
	case INTERMISSION:
		currentGameState = this->updateIntermission();
		break;
	case STARTOFINTERMISSION:
		currentGameState = this->startIntermission();
		break;
	case STARTLANDING:
		currentGameState = this->startLanding();
		break;
	default:
		break;
	}

	//Debug keybinds
	this->handleKeybinds();

	//animations
	this->currentPlayer->updateAnim(dt, 0, 1);

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

	basicRenderer.changeToAnimation();
	currentPlayer->draw();

	//Unbind light
	ltHandler.unbindSrv();

	//Render fresnel objects
	basicRenderer.fresnelPrePass(this->camera);
	this->drawFresnel();

	//Render Skybox
	basicRenderer.skyboxPrePass();
	skybox.draw();
	basicRenderer.depthUnbind();

	//Render imgui & wireframe
	imGui.react3D(wireframe, objectDraw, landingMinigame, dt, velocityCamera);
	if (wireframe) { physWorld.renderReact3D(); playerVecRenderer.drawLines(); }

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