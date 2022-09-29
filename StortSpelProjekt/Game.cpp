#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{
	//Here we can add base object we want in the beginning of the game
	planet = new GameObject("../Meshes/Planet", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(20.0f, 20.0f, 20.0f));
	
	//physWolrd.addPhysComponent(planet, reactphysics3d::CollisionShapeName::BOX);
	//planet->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));

	//skybox = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(-200.0f, 200.0f, 200.0f));
	testCube = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	physWolrd.addPhysComponent(testCube, reactphysics3d::CollisionShapeName::BOX);
	testCube->getPhysComp()->setPosition(reactphysics3d::Vector3(100, 120, 100));
	testCube->getPhysComp()->setParent(testCube);

	for (int i = 0; i < 20; i++)
	{
		GameObject* newObj = new GameObject("../Meshes/Player", DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 1, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		physWolrd.addPhysComponent(newObj, reactphysics3d::CollisionShapeName::BOX);
		newObj->getPhysComp()->setPosition(reactphysics3d::Vector3(-100, 120+i*10, 100));
		testObjects.emplace_back(newObj);
	}

	physWolrd.addPhysComponent(&potion, reactphysics3d::CollisionShapeName::BOX);
	potion.getPhysComp()->setPosition(reactphysics3d::Vector3(potion.getPosV3().x, potion.getPosV3().y, potion.getPosV3().z));

}

void Game::drawObjects()
{
	potion.draw();
	player.draw();
	planet->draw();
	testCube->draw();
	for (int i = 0; i < testObjects.size(); i++)
	{
		testObjects[i]->draw();
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
	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));

	//this->setUpReact3D();

	this->mouse = &mouse;
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{
	wireBuffer->Release();
	delete planet;
	delete testCube;
	for (int i = 0; i < testObjects.size(); i++)
	{
		delete testObjects[i];
	}
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
	potion.getPhysComp()->updateParent();
	testCube->getPhysComp()->updateParent();
	for (int i = 0; i < testObjects.size(); i++)
	{
		testObjects[i]->getPhysComp()->updateParent();
	}
	//testCube->setPos(testCube->getPhysComp()->getPosV3());
	//testCube->setRot(DirectX::XMFLOAT3(testCube->getPhysComp()->getRotation().getVectorV().x, testCube->getPhysComp()->getRotation().getVectorV().y, testCube->getPhysComp()->getRotation().getVectorV().z));
	//testCube->setRot(DirectX::XMQuaternionNormalize(DirectX::XMVectorSet(testCube->getPhysComp()->getRotation().getVectorV().x, testCube->getPhysComp()->getRotation().getVectorV().y, testCube->getPhysComp()->getRotation().getVectorV().z, 1.0f)));
	//Here you can write client-server related functions?

	potion.updateBuffer();
	player.updateBuffer();
	planet->updateBuffer();
	testCube->updateBuffer();
	for (int i = 0; i < testObjects.size(); i++)
	{
		testObjects[i]->updateBuffer();
	}

	mouse->clearEvents();
	
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

	basicRenderer.setUpScene();
	imGui.react3D(wireframe, objectDraw, reactWireframeInfo.wireframeClr, dt);
	if (objectDraw) this->drawObjects();
	if (wireframe) { this->updateBuffers(); immediateContext->PSSetConstantBuffers(0, 1, &wireBuffer), physWolrd.renderReact3D(); }
}
