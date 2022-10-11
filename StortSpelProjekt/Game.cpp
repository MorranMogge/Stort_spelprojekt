#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{

	tmpMesh = new Mesh(this->vBuff, this->iBuff, this->subMeshRanges, this->verticies);
	
	std::cout << subMeshRanges.size() << "\n";
	std::cout << verticies.size() << "\n";

	tmp = new Player(tmpMesh, DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),0);
	
	

	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(20.0f, 20.0f, 20.0f));
	player = new Player("../Meshes/Player", DirectX::SimpleMath::Vector3(22, 12, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);
	potion = new Potion("../Meshes/Baseball", DirectX::SimpleMath::Vector3(10, 10, 15), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0);

	//physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::BOX);
	//planet->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));

	//skybox = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(-200.0f, 200.0f, 200.0f));
	testCube = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	testCube->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));
	testCube->getPhysComp()->setParent(testCube);

	gameObjects.emplace_back(player);
	gameObjects.emplace_back(planet);
	gameObjects.emplace_back(potion);
	gameObjects.emplace_back(testCube);

	for (int i = 0; i < 10; i++)
	{
		GameObject* newObj = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		physWolrd.addPhysComponent(newObj, reactphysics3d::CollisionShapeName::BOX);
		newObj->getPhysComp()->setPosition(reactphysics3d::Vector3(-100, 120+i*10, 100));
		gameObjects.emplace_back(newObj);
	}

	physWolrd.addPhysComponent(potion, reactphysics3d::CollisionShapeName::BOX);
	potion->getPhysComp()->setPosition(reactphysics3d::Vector3(potion->getPosV3().x, potion->getPosV3().y, potion->getPosV3().z));

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
	//for (int i = 0; i < gameObjects.size(); i++)
	//{
	//	gameObjects.at(i)->draw();
	//}

	this->tmp->tmpDraw();
	

	//MaterialLibrary::textures[]
	//UINT stride = sizeof(vertex);
	//UINT offset = 0;
	//
	//int startIndex = 0;
	//int startVertex = 0;
	//GPU::immediateContext->PSSetShaderResources(0, 1, &tempSRV);
	//GPU::immediateContext->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
	//GPU::immediateContext->IASetIndexBuffer(iBuff, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	//for (int i = 0; i < subMeshRanges.size(); i++)
	//{
	//
	//	if (i == 0)
	//	{
	//		GPU::immediateContext->PSSetShaderResources(0, 1, &tempSRV2);
	//	}
	//	else
	//	{
	//		GPU::immediateContext->PSSetShaderResources(0, 1, &tempSRV);
	//	}
	//	
	//	GPU::immediateContext->DrawIndexed(subMeshRanges[i], startIndex, startVertex);
	//	startIndex += subMeshRanges[i];
	//	startVertex += this->verticies[i];
	//}

	//Draw light debug meshes
	if (drawDebug)
	{
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
		gameObjects.at(i)->updateBuffer();
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

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	this->vBuff = {};
	this->iBuff = {};
	this->manager.setDevice(device);
	this->manager.loadMeshData("../Meshes/gob.obj");
	this->manager.getMeshData("../Meshes/gob.obj", vBuff, iBuff, subMeshRanges, verticies);




	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC desc = {};
	int width, height, channels;
	unsigned char* image = stbi_load("../Textures/texture2.png", &width, &height, &channels, STBI_rgb_alpha);
	if (!image)
	{
		stbi_image_free(image);
		
	}
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.MiscFlags = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = image;
	data.SysMemPitch = width * 4; //RBGA - RGBA - ...

	HRESULT hr = device->CreateTexture2D(&desc, &data, &texture);
	stbi_image_free(image);
	hr = device->CreateShaderResourceView(texture, NULL, &tempSRV);
	
	texture->Release();



	
	image = stbi_load("../Textures/Missing.png", &width, &height, &channels, STBI_rgb_alpha);
	if (!image)
	{
		stbi_image_free(image);

	}
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.MiscFlags = 0;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	data = {};
	data.pSysMem = image;
	data.SysMemPitch = width * 4; //RBGA - RGBA - ...

	hr = device->CreateTexture2D(&desc, &data, &texture);
	stbi_image_free(image);
	hr = device->CreateShaderResourceView(texture, NULL, &tempSRV2);

	texture->Release();






	//this->tempSRV = this->manager.getSrv();
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->tmp->setSrv(tempSRV2);
	
	this->setUpWireframe();
	//camera.updateCamera(immediateContext);
	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(20, 30, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ltHandler.addLight(DirectX::XMFLOAT3(10, -20, 30), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 1, 0));
	ptEmitters.push_back(ParticleEmitter(DirectX::XMFLOAT3(0, 0, 20), DirectX::XMFLOAT3(0.5, 0.5, 0), 36, DirectX::XMFLOAT2(2,5)));


	this->mouse = &mouse;
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{

	for (int i = 0; i < this->gameObjects.size(); i++)
	{
		if (this->gameObjects.at(i) != nullptr)
		{
			delete this->gameObjects.at(i);
		}
	}
	
	//delete tmpMesh;
	delete tmp;
	
	wireBuffer->Release();
	this->tempSRV->Release();
	this->tempSRV2->Release();
}

GAMESTATE Game::Update()
{
	mouse->handleEvents(this->window, camera);

	//Do we want this?
	grav = planetGravityField.calcGravFactor(player->getPosV3());
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(player->getPos()));
	player->move(grav, camera.getRightVec(), dt);
	
	//Keeps player at the surface of the planet
	if (getLength(player->getPos()) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); DirectX::XMFLOAT3 tempPos = normalizeXMFLOAT3(player->getPos()); player->setPos(getScalarMultiplicationXMFLOAT3(22, tempPos)); }
	player->movePos(getScalarMultiplicationXMFLOAT3(dt, velocity));
	
	//Player functions
	player->pickupItem(potion);
	player->update();
	
	camera.moveCamera(player->getPosV3(), dt);
	
	physWolrd.updatePlayerBox(player->getPos());
	physWolrd.addForceToObjects();
	physWolrd.update(dt);
	potion->update(); //getPhysComp()->updateParent();
	testCube->update();//getPhysComp()->updateParent();
	for (int i = 4; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();//->getPhysComp()->updateParent();
	}
	//Here you can write client-server related functions?

		this->updateBuffers();
	
	if (player->repairedShip()) { std::cout << "You have repaired the ship and returned to earth\n"; return EXIT; }
	
	mouse->clearEvents();

	//Debug keybinds
	this->handleKeybinds();
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
	if (objectDraw) drawObjects(drawDebug);

	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (wireframe) { immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); }

	//Render Particles
	basicRenderer.geometryPass(camera);
	drawParticles();
	this->ptEmitters.at(0).unbind();


}
