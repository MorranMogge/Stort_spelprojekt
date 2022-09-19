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
	reactphysics3d::Transform playerTransform = reactphysics3d::Transform(reactphysics3d::Vector3(10, 10, -10), reactphysics3d::Quaternion::identity());
	playerRigidBody = world->createRigidBody(playerTransform);
	playerCollider = playerRigidBody->addCollider(playerShape, reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity()));
	playerRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	playerRigidBody->enableGravity(false);
	playerRigidBody->setMass(1000);
	//playerRigidBody->applyLocalTorque(reactphysics3d::Vector3(10000, 10000, 10000));


	//Planet
	planetShape = com.createSphereShape(reactphysics3d::decimal(5));
	reactphysics3d::Transform planetTransform = reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity());
	planetRigidBody = world->createRigidBody(planetTransform);
	planetCollider = planetRigidBody->addCollider(planetShape, planetTransform);
	planetRigidBody->setType(reactphysics3d::BodyType::STATIC);
	planetRigidBody->enableGravity(false);

	world->setIsDebugRenderingEnabled(true);
	debugRenderer = &world->getDebugRenderer();
	debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
	debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
	debugRenderer->computeDebugRenderingPrimitives(*world);
	world->update(0.00001);
	debugRenderer->computeDebugRenderingPrimitives(*world);

}

void Game::renderReact3D()
{
	
	debugRenderer->computeDebugRenderingPrimitives(*world);

	//std::cout << "Nr of lines: " << debugRenderer->getNbLines() << "\nNr of triangles: " << debugRenderer->getNbTriangles() << std::endl;
	//debugRenderer->getLinesArray();
	const reactphysics3d::DebugRenderer::DebugTriangle* triangles = debugRenderer->getTrianglesArray();
	int nrOfTriangles = this->debugRenderer->getNbTriangles();
	immediateContext->IASetVertexBuffers(0, 1, &debuggerBuffer, &stride, &offset);
	immediateContext->VSSetShader(dvShader, nullptr, 0);
	immediateContext->PSSetShader(dpShader, nullptr, 0);
	immediateContext->IASetInputLayout(dInput);
	immediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED);

	immediateContext->Draw(nrOfTriangles, 0);
}

bool Game::setVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(reactphysics3d::DebugRenderer::DebugTriangle) * this->debugRenderer->getTriangles().size();
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	stride = sizeof(reactphysics3d::DebugRenderer::DebugTriangle);
	offset = 0;

	D3D11_SUBRESOURCE_DATA data = {};

	for (int i = 0; i < this->debugRenderer->getTriangles().size(); i++)
	{
		tri.push_back(this->debugRenderer->getTrianglesArray()[i]);
	}
	//data.pSysMem = this->debugRenderer->getTrianglesArray();
	data.pSysMem = tri.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &debuggerBuffer);

	std::string temp;
	LoadVertexShader(GPU::device, dvShader, temp, "DebuggerVshader");
	LoadPixelShader(GPU::device, dpShader, "DebuggerPshader");

	D3D11_INPUT_ELEMENT_DESC inputDesc[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = GPU::device->CreateInputLayout(inputDesc, std::size(inputDesc), temp.c_str(), temp.length(), &dInput);

	return !FAILED(hr);
}

Game::Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain)
	:camera(Camera(immediateContext, device)), immediateContext(immediateContext), velocity(DirectX::XMFLOAT3(0,0,0))
{
	MaterialLibrary::LoadDefault();

	basicRenderer.initiateRenderer(immediateContext, device, swapChain, GPU::windowWidth, GPU::windowHeight);
	this->loadObjects();
	this->setUpReact3D();
	this->setVertexBuffer();
	camera.updateCamera(immediateContext);
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
	dvShader->Release();
	debuggerBuffer->Release();
	dpShader->Release();
	dInput->Release();
}

GAMESTATE Game::Update()
{
	constexpr float speed = 0.3f;
	static bool forward = false;
	float zpos = meshes_Dynamic[0].position.z;

	//DirectX::XMFLOAT3 pos(meshes_Dynamic[0].position.x, meshes_Dynamic[0].position.y, meshes_Dynamic[0].position.z);
	DirectX::XMFLOAT3 pos = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y, playerRigidBody->getTransform().getPosition().z};

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
	playerRigidBody->applyLocalForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(grav.x, grav.y, grav.z));
	world->update(reactphysics3d::decimal(dt));
	meshes_Dynamic[0].position = { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y , playerRigidBody->getTransform().getPosition().z};
	meshes_Dynamic[0].rotation = { playerRigidBody->getTransform().getOrientation().x, playerRigidBody->getTransform().getOrientation().y , playerRigidBody->getTransform().getOrientation().z};

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
	this->renderReact3D();
	dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - start)).count();
}
