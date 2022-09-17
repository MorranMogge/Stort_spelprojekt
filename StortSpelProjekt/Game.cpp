#include "Game.h"
#include "DirectXMathHelper.h"

void Game::loadObjects()
{
	// load obj file
	std::vector<OBJ>objs_Static{
		OBJ("../Meshes/Planet")
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
		OBJ("../Meshes/Player"),
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
	meshes_Dynamic[0].position = DirectX::SimpleMath::Vector3(25, 25, -25);


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
}

void Game::setUpReact3D()
{
	// Create the world settings 
	reactphysics3d::PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 20;
	settings.isSleepingEnabled = false;
	settings.worldName = "Planet SIS";

	// Create the physics world with your settings 
	world = com.createPhysicsWorld(settings);

	//Create Player
	playerShape = com.createBoxShape(reactphysics3d::Vector3(5, 5, 5));
	reactphysics3d::Transform playerTransform = reactphysics3d::Transform(reactphysics3d::Vector3(30, 30, -30), reactphysics3d::Quaternion::identity());
	playerRigidBody = world->createRigidBody(playerTransform);
	playerCollider = playerRigidBody->addCollider(playerShape, playerTransform);
	playerRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	playerRigidBody->enableGravity(false);
	playerRigidBody->setMass(10);
	playerRigidBody->applyLocalTorque(reactphysics3d::Vector3(10000, 10000, 10000));


	//Planet
	planetShape = com.createSphereShape(reactphysics3d::decimal(5));
	reactphysics3d::Transform planetTransform = reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity());
	planetRigidBody = world->createRigidBody(planetTransform);
	planetCollider = planetRigidBody->addCollider(planetShape, planetTransform);
	planetRigidBody->setType(reactphysics3d::BodyType::STATIC);
	planetRigidBody->enableGravity(false);

}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window)
	:camera(Camera(immediateContext, device)), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0))
{
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	//this->setUpReact3D();
	camera.updateCamera(immediateContext);

	this->mouse = &mouse;
	this->window = &window;
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{
	if (playerRigidBody != nullptr) world->destroyRigidBody(playerRigidBody);
	if (planetRigidBody != nullptr) world->destroyRigidBody(planetRigidBody);
	if (playerShape != nullptr) com.destroyBoxShape(playerShape);
	if (planetShape != nullptr) com.destroySphereShape(planetShape);
	if (world != nullptr) com.destroyPhysicsWorld(world);
}

GAMESTATE Game::Update()
{
	//Mouse events
	if (PeekMessage(msg, NULL, WM_MOUSEFIRST, WM_KEYLAST, PM_REMOVE))
	{
		TranslateMessage(msg);
		DispatchMessage(msg);
	}

	while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
	{
	}

	ZeroMemory(&mousePos, sizeof(POINT));
	if (GetCursorPos(&mousePos))
	{
		if (ScreenToClient(*window, &mousePos))
		{
		}
	}

	while (!mouse->EventBufferIsEmpty())
	{
		MouseEvent me = mouse->ReadEvent();

		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			camera.AdjustRotation(immediateContext, (float)me.GetPosY() * .0025f, (float)me.GetPosX() * .0025f);
			GetClientRect(*window, &rect);
			ul.x = rect.left;
			ul.y = rect.top;

			lr.x = rect.right;
			lr.y = rect.bottom;

			MapWindowPoints(*window, nullptr, &ul, 1);
			MapWindowPoints(*window, nullptr, &lr, 1);

			rect.left = ul.x;
			rect.top = ul.y;

			rect.right = lr.x;
			rect.bottom = lr.y;

			ClipCursor(&rect);
		}
	}

	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	grav = normalizeXMFLOAT3(grav);
	playerUpVec = DirectX::XMVectorSet(-grav.x, -grav.y, -grav.z, 0.0f);
	playerForwardVec = DirectX::XMVector3Cross(camera.getRightVec(), playerUpVec);
	playerRightVec = DirectX::XMVector3Cross(playerUpVec, playerForwardVec);


	/*XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, playerForwardVec);

	OutputDebugString(L"PLAYER FORWARD: ");
	OutputDebugString(std::to_wstring(newPos.x).c_str());
	OutputDebugString(L"\n");
	OutputDebugString(std::to_wstring(newPos.y).c_str());
	OutputDebugString(L"\n");
	OutputDebugString(std::to_wstring(newPos.z).c_str());
	OutputDebugString(L"\n");
	OutputDebugString(L"\n");*/

	if (Input::KeyDown(KeyCode::W))
	{
		//meshes_Dynamic[0].position += playerForwardVec;

		XMFLOAT3 newPos;
		XMStoreFloat3(&newPos, playerForwardVec);

		meshes_Dynamic[0].rotation.y;
	}

	else if (Input::KeyDown(KeyCode::S))
	{
		meshes_Dynamic[0].position -= playerForwardVec;
	}

	if (Input::KeyDown(KeyCode::D))
	{
		meshes_Dynamic[0].position += playerRightVec;
	}

	else if (Input::KeyDown(KeyCode::A))
	{
		meshes_Dynamic[0].position -= playerRightVec;
	}

	if (Input::KeyDown(KeyCode::E))
	{
		meshes_Dynamic[0].position.y += 0.1;
	}

	else if (Input::KeyDown(KeyCode::Q))
	{
		meshes_Dynamic[0].position.y -= 0.1;
	}

	DirectX::XMFLOAT3 pos(meshes_Dynamic[0].position.x, meshes_Dynamic[0].position.y, meshes_Dynamic[0].position.z);
	grav = planetGravityField.calcGravFactor(pos);

	/*OutputDebugString(L"PLAYER: ");
	OutputDebugString(std::to_wstring(pos[2]).c_str());
	OutputDebugString(L"\n");*/

	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(pos));
	if (getLength(pos) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); newNormalizeXMFLOAT3(pos); scalarMultiplicationXMFLOAT3(22, pos); }
	additionXMFLOAT3(pos, getScalarMultiplicationXMFLOAT3(dt, velocity));
	meshes_Dynamic[0].position = { pos.x, pos.y, pos.z };

	camera.moveCamera(immediateContext, meshes_Dynamic[0].position, playerForwardVec, playerUpVec, dt);

	//KLARA DONT LOOK HERE!
	//DirectX::XMFLOAT3 pos = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y, playerRigidBody->getTransform().getPosition().z};
	//playerRigidBody->applyLocalForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(grav.x, grav.y, grav.z));
	//world->update(reactphysics3d::decimal(dt));
	//meshes_Dynamic[0].position = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y , playerRigidBody->getTransform().getPosition().z};
	//meshes_Dynamic[0].rotation = { playerRigidBody->getTransform().getOrientation().x, playerRigidBody->getTransform().getOrientation().y , playerRigidBody->getTransform().getOrientation().z};

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}

	mouse->clearEvents();
	return NOCHANGE;
}

void Game::Render()
{
	start = std::chrono::system_clock::now();
	basicRenderer.setUpScene();
	drawObjects();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}
