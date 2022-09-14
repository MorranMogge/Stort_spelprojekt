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
	///meshes_Static[0].scale = DirectX::SimpleMath::Vector3(5, 5, 5);

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
	meshes_Dynamic[0].scale = DirectX::SimpleMath::Vector3(0.25, 0.25, 0.25);
	meshes_Dynamic[0].position = DirectX::SimpleMath::Vector3(10, 10, -10);


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
	playerCollider = com.createBoxShape(reactphysics3d::Vector3(1,1,1));
	reactphysics3d::Transform playerTransform = reactphysics3d::Transform(reactphysics3d::Vector3(10, 10, 10), reactphysics3d::Quaternion::identity());
	playerRigidBody = world->createRigidBody(playerTransform);
	playerRigidBody->addCollider(playerCollider, playerTransform);
	playerRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	playerRigidBody->enableGravity(false);

	//Planet
	planetCollider = com.createSphereShape(reactphysics3d::decimal(1));
	reactphysics3d::Transform planetTransform = reactphysics3d::Transform::identity();
	planetRigidBody = world->createRigidBody(planetTransform);
	planetRigidBody->addCollider(planetCollider, planetTransform);
	planetRigidBody->setType(reactphysics3d::BodyType::STATIC);
	planetRigidBody->enableGravity(false);

}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain)
	:camera(Camera(immediateContext, device)), immediateContext(immediateContext)
{
	MaterialLibrary::LoadDefault();
	/*UINT WIDTH = 1264;
	UINT HEIGHT = 681;*/

	UINT WIDTH = 1280;
	UINT HEIGHT = 720;

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpReact3D();
	camera.updateCamera(immediateContext);
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{
	if (playerRigidBody != nullptr) world->destroyRigidBody(playerRigidBody);
	if (planetRigidBody != nullptr) world->destroyRigidBody(planetRigidBody);
	if (playerCollider != nullptr) com.destroyBoxShape(playerCollider);
	if (planetCollider != nullptr) com.destroySphereShape(planetCollider);
	if (world != nullptr) com.destroyPhysicsWorld(world);
}

GAMESTATE Game::Update()
{
	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	//DirectX::XMFLOAT3 pos( meshes_Dynamic[0].position.x,meshes_Dynamic[0].position.y ,meshes_Dynamic[0].position.z );
	DirectX::XMFLOAT3 pos = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y, playerRigidBody->getTransform().getPosition().z};

	if (Input::KeyDown(KeyCode::W)) pos.z += 0.1;
	if (Input::KeyDown(KeyCode::S)) pos.z -= 0.1;
	if (Input::KeyDown(KeyCode::I)) pos.y += 0.1;
	if (Input::KeyDown(KeyCode::K)) pos.y -= 0.1;
	if (Input::KeyDown(KeyCode::D)) pos.x += 0.1;
	if (Input::KeyDown(KeyCode::A)) pos.x -= 0.1;

	/*for (int i = 0; i < 3; i++)
	{
		if (pos[i] > 10) pos[i] = 10;
		else if (pos[i] < -10) pos[i] = -10;
	}*/

	//APPLY FORCE TO THE RIGID BODY
	//subtractionXMFLOAT3(pos, planetGravityField.calcGravFactor(pos));
	DirectX::XMFLOAT3 grav = planetGravityField.calcGravFactor(pos);
	playerRigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(grav.x, grav.y, grav.z));

	world->update(reactphysics3d::decimal(dt));

	meshes_Dynamic[0].position = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y , playerRigidBody->getTransform().getPosition().z};
	meshes_Static[0].rotation.y += 0.0001;
	meshes_Static[0].rotation.x += 0.0001;


	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}

	//camera.moveCamera(immediateContext, 1.f/100.f);
	//std::cout << "Name of the world: " << world->getName() << std::endl;
	return NOCHANGE;
}

void Game::Render()
{
	start = std::chrono::system_clock::now();
	basicRenderer.setUpScene();
	drawObjects();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}
