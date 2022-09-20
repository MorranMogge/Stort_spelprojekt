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
	meshes_Static[0].scale = DirectX::SimpleMath::Vector3(2.5,2.5,2.5);

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
	meshes_Dynamic[0].position = DirectX::SimpleMath::Vector3(25, 25, -25);


}

void Game::drawObjects()
{
	//for (auto& mesh : meshes_Static) //draw Static meshes
	//{
	//	mesh.DrawWithMat();
	//}
	for (auto& mesh : meshes_Dynamic) //draw Dynamic meshes
	{
		mesh.DrawWithMat();
	}
}



Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain)
	:camera(Camera(immediateContext, device)), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0,0,0))
{
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	camera.updateCamera(immediateContext);
	start = std::chrono::system_clock::now();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}

Game::~Game()
{
}

GAMESTATE Game::Update()
{
	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	DirectX::XMFLOAT3 pos(meshes_Dynamic[0].position.x, meshes_Dynamic[0].position.y, meshes_Dynamic[0].position.z);
	//DirectX::XMFLOAT3 pos = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y, playerRigidBody->getTransform().getPosition().z};

	if (Input::KeyDown(KeyCode::W)) pos.z += 0.1;
	if (Input::KeyDown(KeyCode::S)) pos.z -= 0.1;
	if (Input::KeyDown(KeyCode::I)) pos.y += 0.1;
	if (Input::KeyDown(KeyCode::K)) pos.y -= 0.1;
	if (Input::KeyDown(KeyCode::D)) pos.x += 0.1;
	if (Input::KeyDown(KeyCode::A)) pos.x -= 0.1;

	grav = planetGravityField.calcGravFactor(pos);

	/*additionXMFLOAT3(velocity, planetGravityField.calcGravFactor(pos));
	if (getLength(pos) <= 20+2) velocity = DirectX::XMFLOAT3(0, 0, 0);
	additionXMFLOAT3(pos, getScalarMultiplicationXMFLOAT3(dt,velocity));
	meshes_Dynamic[0].position = { pos.x, pos.y, pos.z };*/

	//KLARA DONT LOOK HERE!
	//playerRigidBody->applyLocalForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(grav.x, grav.y, grav.z));
	//world->update(reactphysics3d::decimal(dt));
	
	physWolrd.addForceToObject(grav);
	physWolrd.update(dt);
	meshes_Dynamic[0].position = physWolrd.getPos();
	meshes_Dynamic[0].rotation = physWolrd.getRot();

	for (int i = 0; i < meshes_Static.size(); i++)
	{
		meshes_Static[i].UpdateCB();
	}

	for (int i = 0; i < meshes_Dynamic.size(); i++)
	{
		meshes_Dynamic[i].UpdateCB();
	}

	//camera.moveCamera(immediateContext, 1.f/100.f);
	return NOCHANGE;
}

void Game::Render()
{
	start = std::chrono::system_clock::now();
	basicRenderer.setUpScene();
	this->drawObjects();
	physWolrd.renderReact3D();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}
