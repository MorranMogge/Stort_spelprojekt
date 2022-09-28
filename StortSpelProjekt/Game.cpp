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

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window, Client*& client)
	:camera(Camera()), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0, 0, 0)), player("../Meshes/Player",
		DirectX::SimpleMath::Vector3(22, 22, -22), DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), 0)
{
	MaterialLibrary::LoadDefault();

	this->client = client;

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	ltHandler.addLight(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT3(0, 1, 0));

	//this->setUpReact3D();

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
	//mouse->handleEvents(this->window, camera);

	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	grav = normalizeXMFLOAT3(grav);
	player.move(meshes_Dynamic[0].position, meshes_Dynamic[0].rotation, grav, camera.getRightVec(), dt);
	grav = planetGravityField.calcGravFactor(meshes_Dynamic[0].position);
	additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(meshes_Dynamic[0].position));
	if (getLength(meshes_Dynamic[0].position) <= 22) { velocity = DirectX::XMFLOAT3(0, 0, 0); newNormalizeXMFLOAT3(meshes_Dynamic[0].position); scalarMultiplicationXMFLOAT3(22, meshes_Dynamic[0].position); }
	additionXMFLOAT3(meshes_Dynamic[0].position, getScalarMultiplicationXMFLOAT3(dt, velocity));
	camera.moveCamera(meshes_Dynamic[0].position, dt);

	//KLARA DONT LOOK HERE!
	//DirectX::XMFLOAT3 pos = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y, playerRigidBody->getTransform().getPosition().z};
	//playerRigidBody->applyLocalForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(grav.x, grav.y, grav.z));
	//world->update(reactphysics3d::decimal(dt));
	//meshes_Dynamic[0].position = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y , playerRigidBody->getTransform().getPosition().z};
	//meshes_Dynamic[0].rotation = { playerRigidBody->getTransform().getOrientation().x, playerRigidBody->getTransform().getOrientation().y , playerRigidBody->getTransform().getOrientation().z};


	if (client->getIsConnected())
	{
		client->sendToServerPlayerPos(player);
	}

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}
	
	return NOCHANGE;
}

void Game::Render()
{
	start = std::chrono::system_clock::now();
	//LIGHT STUFF
	basicRenderer.lightPrePass();
	for (int i = 0; i < ltHandler.getNrOfLights(); i++)
	{
		ltHandler.drawShadows(i, gameObjects);
	}
	ltHandler.bindLightBuffers();

	basicRenderer.setUpScene();
	drawObjects();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}
