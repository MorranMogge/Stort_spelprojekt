#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{
	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(20.0f, 20.0f, 20.0f));
}

void Game::drawObjects()
{
	potion.draw();
	currentPlayer->draw();
	for (int i = 0; i < NROFPLAYERS; i++)
	{
		players[i]->draw();
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
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(wireBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &reactWireframeInfo, sizeof(wirefameInfo));
	immediateContext->Unmap(wireBuffer, 0);
}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window, ImGuiHelper& imguiHelper, Client*& client)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0)), potion("../Meshes/player", DirectX::SimpleMath::Vector3(10,10,15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0)
{
	
	MaterialLibrary::LoadDefault();

	this->client = client;
	//måste raderas******************
	circularBuffer = client->getCircularBuffer();


	this->imguiHelper = &imguiHelper;
	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpWireframe();
	//camera.updateCamera(immediateContext);
	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));

	//this->setUpReact3D();

	currentPlayer = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);

	//TEMPORÄR GÖRA MER DYNAMISK SENARE
	client->connectToServer();
	int playerid = client->initTEMPPLAYERS();

	for (int i = 0; i < NROFPLAYERS; i++)//initialize players 
	{
		Player *tmpPlayer = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
		if (playerid != i)
		{
			players.push_back(tmpPlayer);
		}
		else
		{
			players.push_back(currentPlayer);
			delete tmpPlayer;
		}
	}

	serverStart = std::chrono::system_clock::now();
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{
	//delete client;
	wireBuffer->Release();
	delete planet;
}

GAMESTATE Game::Update()
{

	if (GetAsyncKeyState('C')) physWolrd.addBoxToWorld();
	//Do we want this?
	grav = planetGravityField.calcGravFactor(currentPlayer->getPosV3());
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(currentPlayer->getPos()));
	currentPlayer->move(grav, camera.getRightVec(), dt);
	
	//Keeps player at the surface of the planet
	if (getLength(currentPlayer->getPos()) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); DirectX::XMFLOAT3 tempPos = normalizeXMFLOAT3(currentPlayer->getPos()); currentPlayer->setPos(getScalarMultiplicationXMFLOAT3(22, tempPos)); }
	currentPlayer->movePos(getScalarMultiplicationXMFLOAT3(dt, velocity));
	
	currentPlayer->pickupItem(&potion);
	currentPlayer->update();
	
	camera.moveCamera(currentPlayer->getPosV3(), dt);
	
	physWolrd.updatePlayerBox(currentPlayer->getPos());
	physWolrd.addForceToObjects();
	physWolrd.update(dt);
	
	/*if (client->getChangePlayerPos())
	{
		movePlayerEvent	tmpMovePlayer = client->getMovePlayerEvent();
		players.at(tmpMovePlayer.affectedPlayerId)->setPos(DirectX::XMFLOAT3(tmpMovePlayer.pos[0], tmpMovePlayer.pos[1], tmpMovePlayer.pos[2]));
		client->setPlayerRecv(false);
	}*/
	if (((std::chrono::duration<float>)(std::chrono::system_clock::now() - serverStart)).count() > serverTimerLength && client->getIfConnected())
	{
		client->sendToServerTEMPTCP(currentPlayer);
		serverStart = std::chrono::system_clock::now();
	}

	//circbuffer tests
	
	std::string packetsLeft = "false";
	
	if (circularBuffer->getIfPacketsLeftToRead())packetsLeft = "true";
	//std::cout << "check if circbuffer have packets = " << packetsLeft << std::endl;
	circularBuffer->debugWriteMemoryAdress();
	//körs till det inte finns packet kvar att läsa
	while (circularBuffer->getIfPacketsLeftToRead())
	{
		int packetId = circularBuffer->getPacketId();
		//std::cout << "Game.cpp packet id: " << std::to_string(packetId) << std::endl;

		if (packetId == 4)
		{
			idProtocol* protocol = circularBuffer->readData<idProtocol>();
			//idProtocol* protocol = (idProtocol*)circularBuffer->getData();
			//circularBuffer->advancePointer(*protocol);
			std::cout << "received player id: " << std::to_string(protocol->assignedPlayerId) << std::endl;
		}
		else if (packetId == 10)
		{
			testPosition* tst = circularBuffer->readData<testPosition>();
			for (int i = 0; i < NROFPLAYERS; i++)
			{
				if (i == tst->playerId)
				{
					if (client->getPlayerId() != i)
					{
						players[i]->setPos(DirectX::XMFLOAT3(tst->x, tst->y, tst->z));
					}
				}
			}
			/*testPosition* tst = (testPosition*)circularBuffer->getData();
			circularBuffer->advancePointer(*tst);*/

			std::cout << "Checking return value from circular buffer player id.x: " << std::to_string(tst->playerId) << " y: " << std::to_string(tst->y) << std::endl;
			//std::cout << "Player id: " << std::to_string(tst->playerId) << std::endl;
		}
	}
	
	for (int i = 0; i < NROFPLAYERS; i++)
	{
		players[i]->updateBuffer();
	}
	//end of circBuffer tests


	potion.updateBuffer();
	currentPlayer->updateBuffer();
	planet->updateBuffer();
	
	
	return NOCHANGE;
}

void Game::Render()
{
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
	start = std::chrono::system_clock::now();
	//LIGHT STUFF
	basicRenderer.lightPrePass();
	for (int i = 0; i < ltHandler.getNrOfLights(); i++)
	{
		ltHandler.drawShadows(i, gameObjects);
	}
	ltHandler.bindLightBuffers();

	for (int i = 0; i < NROFPLAYERS; i++)
	{
		players[i]->draw();
	}

	basicRenderer.setUpScene();
	if (objectDraw) this->drawObjects();
	if (wireframe) { this->updateBuffers(); immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); }

	//imguiHelper->drawInterface("test");
}
