#include "Game.h"
#include "DirectXMathHelper.h"



void Game::loadObjects()
{
	// load obj file
	std::vector<OBJ>objs_Static{
		OBJ("../Meshes/Planet"),

	};

	// foreach obj in objs_static variable
	for (auto& obj : objs_Static)
	{
		meshes_Static.emplace_back(obj); // create mesh from obj

		// load obj material
		for (auto& mat : obj.mtl.materials)
		{
			MaterialLibrary::LoadMaterial(mat);
		}

	}
	meshes_Static[0].scale = DirectX::SimpleMath::Vector3(20, 20, 20);

	// re-calculate bounding box
	for (auto& mesh : meshes_Static)
	{
		mesh.CalcBound();
	}

	// load obj file
	std::vector<OBJ>objs_Dynamic{
		OBJ("../Meshes/Player")
	};

	// foreach obj in objs_Dynamic variable
	for (auto& obj : objs_Dynamic)
	{
		auto& mesh = meshes_Dynamic.emplace_back(obj); // create mesh from obj

		// load material
		for (auto& mat : obj.mtl.materials)
		{
			MaterialLibrary::LoadMaterial(mat);
		}

	}
	meshes_Dynamic[0].scale = DirectX::SimpleMath::Vector3(1, 1, 1);
	meshes_Dynamic[0].position = DirectX::SimpleMath::Vector3(22, 22, -22);
	
}

void Game::drawObjects()
{
	for (auto& mesh : meshes_Static) //draw Static meshes
	{
		mesh.DrawWithMat();
	}
	for (auto& mesh : meshes_Dynamic) //draw Dynamic meshes
	{
		mesh.DrawWithMat();
	}
	potion.draw();
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

	gamePad = std::make_unique<GamePad>();

	this->mouse = &mouse;
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{
	wireBuffer->Release();
}

GAMESTATE Game::Update()
{
	mouse->handleEvents(this->window, camera);

	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;


	if (GetAsyncKeyState('C')) physWolrd.addBoxToWorld();
	//Do we want this?
	DirectX::XMFLOAT3 pos = { meshes_Dynamic[0].position.x ,  meshes_Dynamic[0].position.y ,  meshes_Dynamic[0].position.z };
	grav = planetGravityField.calcGravFactor(pos);
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(pos));

	grav = normalizeXMFLOAT3(grav);
	player.move(meshes_Dynamic[0].position, meshes_Dynamic[0].rotation, grav, camera.getRightVec(), dt);
	grav = planetGravityField.calcGravFactor(meshes_Dynamic[0].position);
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(meshes_Dynamic[0].position));
	if (getLength(meshes_Dynamic[0].position) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); newNormalizeXMFLOAT3(meshes_Dynamic[0].position); scalarMultiplicationXMFLOAT3(22, meshes_Dynamic[0].position); }
	additionXMFLOAT3(meshes_Dynamic[0].position, getScalarMultiplicationXMFLOAT3(dt, velocity));
	camera.moveCamera(meshes_Dynamic[0].position, dt);

	player.pickupItem(&potion);
	
	player.setPos({ meshes_Dynamic[0].position.x,  meshes_Dynamic[0].position.y, meshes_Dynamic[0].position.z });
	player.update();
	
	auto state = gamePad->GetState(0);

	if (state.IsConnected())
	{
		if (state.IsAPressed())
		{ 
			std::cout << "A is pressed\n";
			gamePad->SetVibration(0, 1.f, 1.f);
		}
		else
		{
			gamePad->SetVibration(0, 0.f, 0.f);

		}

			// Do action for button A being down

		if (state.buttons.y)
		{
			std::cout << "Y is pressed\n";
		}
				// Do action for button Y being down

		if (state.IsDPadLeftPressed())
		{
			std::cout << "DPAD Left is down\n";
		}
					// Do action for DPAD Left being down

		if (state.dpad.up || state.dpad.down || state.dpad.left || state.dpad.right)
		{
			std::cout << "DPAD\n";
		}
						// Do action based on any DPAD change

		float posx = state.thumbSticks.leftX;
		float posy = state.thumbSticks.leftY;
		// These values are normalized to -1 to 1

		if (posy >= 1)
		{
			std::cout << "Y >= 1\n";
		}

		float throttle = state.triggers.right;
		// This value is normalized 0 -> 1

		if (state.IsLeftTriggerPressed())
		{
			std::cout << "Left trigger pressed\n";
		}
			// Do action based on a left trigger pressed more than halfway

		if (state.IsViewPressed())
		{
			std::cout << "backbutton is pressed\n";
		}

		if (state.IsLeftStickPressed())
		{
			std::cout << "Left stick down\n";
		}

		

				// This is an alias for the Xbox 360 'Back' button
				// which is called 'View' on the Xbox One controller.
	}

	
	physWolrd.updatePlayerBox(meshes_Dynamic[0].position);
	physWolrd.addForceToObject(grav);
	physWolrd.update(dt);
	

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}
	potion.updateBuffer();
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
