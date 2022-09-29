#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{
	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(20.0f, 20.0f, 20.0f));
}

void Game::drawShadows()
{
	for (int i = 0; i < ltHandler.getNrOfLights(); i++)
	{
		ltHandler.drawShadows(i, gameObjects);
	}
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
	

	potion.draw();
	player.draw();
	planet->draw();

	
	//Draw light debug meshes
	if (drawDebug)
	{
		ltHandler.drawDebugMesh();
	}
	
	//Remove
	//for (auto& mesh : meshes_Static) //draw Static meshes
	//{
	//	mesh.DrawWithMat();
	//}
	//for (auto& mesh : meshes_Dynamic) //draw Dynamic meshes
	//{
	//	mesh.DrawWithMat();
	//}


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



		if (this->ptEmitters.at(i).isActive() && this->ptEmitters.at(i).isPassComplete())
		{
			
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
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(wireBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &reactWireframeInfo, sizeof(wirefameInfo));
	immediateContext->Unmap(wireBuffer, 0);
}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0)), player("../Meshes/Player", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0), potion("../Meshes/player", DirectX::SimpleMath::Vector3(10,10,15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0)
{
	
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpWireframe();
	//camera.updateCamera(immediateContext);
	//ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(20, 30, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(10, -20, 30), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2,5)));
	gameObjects.push_back(new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, -0), DirectX::XMFLOAT3(1, 0, 0), 0, DirectX::XMFLOAT3(20, 20, 20)));
	gameObjects.push_back(new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(22, 22, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0));


	

	this->mouse = &mouse;
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{

	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		delete this->gameObjects.at(i);
	}

	wireBuffer->Release();
	delete planet;

}

GAMESTATE Game::Update()
{
	mouse->handleEvents(this->window, camera);

	if (GetAsyncKeyState('C')) physWolrd.addBoxToWorld();
	//Do we want this?
	grav = planetGravityField.calcGravFactor(player.getPosV3());
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(player.getPos()));
	player.move(grav, camera.getRightVec(), dt);
	
	//Keeps player at the surface of the planet
	if (getLength(player.getPos()) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); DirectX::XMFLOAT3 tempPos = normalizeXMFLOAT3(player.getPos()); player.setPos(getScalarMultiplicationXMFLOAT3(22, tempPos)); }
	player.movePos(getScalarMultiplicationXMFLOAT3(dt, velocity));
	
	player.pickupItem(&potion);
	player.update();
	
	camera.moveCamera(player.getPosV3(), dt);
	
	physWolrd.updatePlayerBox(player.getPos());
	physWolrd.addForceToObjects();
	physWolrd.update(dt);
	

	//Here you can write client-server related functions?

	potion.updateBuffer();
	player.updateBuffer();
	planet->updateBuffer();
	
	mouse->clearEvents();

	if (Input::KeyPress(KeyCode::T))
	{
		XMFLOAT3 test(this->ptEmitters.at(0).getPosition().x, this->ptEmitters.at(0).getPosition().y, this->ptEmitters.at(0).getPosition().z -10);
		this->ptEmitters.at(0).setPosition(test);
		this->ptEmitters.at(0).setRotation(XMFLOAT3(-1,0,0));
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
	drawObjects(true);


	//Render Particles
	basicRenderer.geometryPass(camera);
	drawParticles();
	this->ptEmitters.at(0).unbind();


	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	//if (objectDraw) this->drawObjects();
	//if (wireframe) { this->updateBuffers(); immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); }
}
