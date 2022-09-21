#include "PhysicsWorld.h"
#include "GPU.h"
#include "ShaderLoader.h"
#include <time.h>
#include "DirectXMathHelper.h"

void PhysicsWorld::setUpBaseScenario()
{
	//Create Player
	playerShape = com.createBoxShape(reactphysics3d::Vector3(0.35f, 0.35f, 0.35f));
	reactphysics3d::Transform playerTransform = reactphysics3d::Transform(reactphysics3d::Vector3(1, 1, 1), reactphysics3d::Quaternion::identity());
	playerRigidBody = world->createRigidBody(playerTransform);
	playerCollider = playerRigidBody->addCollider(playerShape, reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity()));
	playerRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	playerRigidBody->enableGravity(false);
	playerRigidBody->setMass(1);
	playerRigidBody->setTransform(reactphysics3d::Transform(reactphysics3d::Vector3(-10, 10, -20), reactphysics3d::Quaternion::identity()));

	//Planet
	planetShape = com.createSphereShape(reactphysics3d::decimal(5));
	reactphysics3d::Transform planetTransform = reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity());
	planetRigidBody = world->createRigidBody(planetTransform);
	planetCollider = planetRigidBody->addCollider(planetShape, reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity()));
	planetRigidBody->setType(reactphysics3d::BodyType::STATIC);
	planetRigidBody->enableGravity(false);

	world->setIsDebugRenderingEnabled(true);
	debugRenderer = &world->getDebugRenderer();
	debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	debugRenderer->computeDebugRenderingPrimitives(*world);
}

void PhysicsWorld::updateVertexBuffer()
{
	//debugRenderer->computeDebugRenderingPrimitives(*world);
	const reactphysics3d::DebugRenderer::DebugTriangle* debugTriangles = this->debugRenderer->getTrianglesArray(); //If save points (Vector3) as pointer maybe you do not have to copy every frame
	for (int i = 0; i < this->debugRenderer->getTriangles().size(); i++) //We copy the triangle data every frame
	{
		this->triangles[3 * i + 0].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point1.x, debugTriangles[i].point1.y, debugTriangles[i].point1.z);
		this->triangles[3 * i + 1].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point2.x, debugTriangles[i].point2.y, debugTriangles[i].point2.z);
		this->triangles[3 * i + 2].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point3.x, debugTriangles[i].point3.y, debugTriangles[i].point3.z);
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	GPU::immediateContext->Map(debuggerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, this->triangles.data(), this->triangles.size()*sizeof(Vertex));
	GPU::immediateContext->Unmap(debuggerBuffer, 0);
}

bool PhysicsWorld::recreateVertexBuffer()
{
	debuggerBuffer->Release();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(Vertex) * this->triangles.size();
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	stride = sizeof(Vertex);
	offset = 0;

	D3D11_SUBRESOURCE_DATA data = {};

	data.pSysMem = triangles.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &debuggerBuffer);
	return !FAILED(hr);
}

bool PhysicsWorld::setUpWireframe()
{
	D3D11_RASTERIZER_DESC wireframedesc;
	wireframedesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	wireframedesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	wireframedesc.FrontCounterClockwise = false;
	wireframedesc.DepthBias = 0;
	wireframedesc.DepthBiasClamp = 0.0f;
	wireframedesc.SlopeScaledDepthBias = 0.0f;
	wireframedesc.DepthClipEnable = true;
	wireframedesc.ScissorEnable = false;
	wireframedesc.MultisampleEnable = false;
	wireframedesc.AntialiasedLineEnable = false;


	HRESULT hr = GPU::device->CreateRasterizerState(&wireframedesc, &wireframeMode);

	return !FAILED(hr);
}

bool PhysicsWorld::setVertexBuffer()
{
	DirectX::XMStoreFloat4x4(&identityM, DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity()));

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(identityM);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data = {};

	data.pSysMem = &identityM;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &identityMatrix);

	const reactphysics3d::DebugRenderer::DebugTriangle* debugTriangles = this->debugRenderer->getTrianglesArray();
	for (int i = 0; i < this->debugRenderer->getTriangles().size(); i++)
	{
		Vertex newVertex[3];
		newVertex[0].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point1.x, debugTriangles[i].point1.y, debugTriangles[i].point1.z);
		triangles.emplace_back(newVertex[0]);
		newVertex[1].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point2.x, debugTriangles[i].point2.y, debugTriangles[i].point2.z);
		triangles.emplace_back(newVertex[1]);
		newVertex[2].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point3.x, debugTriangles[i].point3.y, debugTriangles[i].point3.z);
		triangles.emplace_back(newVertex[2]);
	}

	bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(Vertex) * this->triangles.size();
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	/*bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;*/
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	stride = sizeof(Vertex);
	offset = 0;

	data = {};

	data.pSysMem = triangles.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	hr = GPU::device->CreateBuffer(&bufferDesc, &data, &debuggerBuffer);

	LoadPixelShader(GPU::device, dpShader, "DebuggerPshader");

	/*D3D11_INPUT_ELEMENT_DESC inputDesc[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = GPU::device->CreateInputLayout(inputDesc, std::size(inputDesc), temp.c_str(), temp.length(), &dInput);*/

	return !FAILED(hr);
}

PhysicsWorld::PhysicsWorld(std::string worldName)
{
	// Create the world settings 
	reactphysics3d::PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 20;
	settings.isSleepingEnabled = false;
	settings.worldName = worldName;

	// Create the physics world with your settings 
	world = com.createPhysicsWorld(settings);
	this->setUpBaseScenario();
	this->setVertexBuffer();
	this->setUpWireframe();
}

void PhysicsWorld::renderReact3D()
{
	this->updateVertexBuffer();
	GPU::immediateContext->VSSetConstantBuffers(0, 1, &identityMatrix);
	GPU::immediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	GPU::immediateContext->IASetVertexBuffers(0, 1, &debuggerBuffer, &stride, &offset);
	GPU::immediateContext->RSSetState(wireframeMode);
	//GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GPU::immediateContext->PSSetShader(dpShader, nullptr, 0);
	GPU::immediateContext->Draw(this->triangles.size(), 0);
	GPU::immediateContext->RSSetState(nullptr);
}

PhysicsWorld::~PhysicsWorld()
{
	if (playerRigidBody != nullptr) world->destroyRigidBody(playerRigidBody);
	if (planetRigidBody != nullptr) world->destroyRigidBody(planetRigidBody);
	if (playerShape != nullptr) com.destroyBoxShape(playerShape);
	if (planetShape != nullptr) com.destroySphereShape(planetShape);
	if (world != nullptr) com.destroyPhysicsWorld(world);
	debuggerBuffer->Release();
	dpShader->Release();
	identityMatrix->Release();
	wireframeMode->Release();
}

void PhysicsWorld::update(float dt)
{
	world->update(dt);
}

void PhysicsWorld::addForceToObject(const DirectX::XMFLOAT3& gravityVec)
{
	playerRigidBody->applyWorldForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(gravityVec.x, gravityVec.y, gravityVec.z));
	/*DirectX::XMFLOAT3 grav;
	reactphysics3d::Vector3 temp;
	for (int i = 0; i < this->bodies.size(); i++)
	{
		temp = this->bodies[i]->getTransform().getPosition();
		grav = normalizeXMFLOAT3(DirectX::XMFLOAT3(-temp.x, -temp.y, -temp.z));
		this->bodies[i]->applyWorldForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(grav.x, grav.y, grav.z));
	}*/
	//playerRigidBody->applyLocalForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(gravityVec.x, gravityVec.y, gravityVec.z));
}

DirectX::SimpleMath::Vector3 PhysicsWorld::getPos()
{
	return { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y , playerRigidBody->getTransform().getPosition().z };
}

DirectX::SimpleMath::Vector3 PhysicsWorld::getRot()
{
	return { playerRigidBody->getTransform().getOrientation().x, playerRigidBody->getTransform().getOrientation().y, playerRigidBody->getTransform().getOrientation().z };
}

void PhysicsWorld::addBoxToWorld(DirectX::XMFLOAT3 dimensions, float mass, DirectX::XMFLOAT3 position)
{
	//if (position.x == 0 && position.y == 0 && position.z == 0) position = DirectX::XMFLOAT3(10 - rand() % 11,  10 - rand() % 11, 10 - rand() % 11);
	int vectorSize = this->bodies.size();
	this->shapes.push_back(com.createBoxShape(reactphysics3d::Vector3(dimensions.x, dimensions.y, dimensions.z)));
	reactphysics3d::Transform transform = reactphysics3d::Transform(reactphysics3d::Vector3(position.x, position.y, position.z), reactphysics3d::Quaternion::identity());
	this->bodies.push_back(world->createRigidBody(transform));
	this->colliders.push_back(this->bodies[vectorSize]->addCollider(this->shapes[vectorSize], reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity())));
	this->bodies[vectorSize]->setType(reactphysics3d::BodyType::DYNAMIC);
	this->bodies[vectorSize]->setMass(mass);

	//We disable gravity since we have implemented our own defined gravity
	//this->bodies[vectorSize]->enableGravity(false);

	this->triangles.clear();
	this->debugRenderer->reset();
	this->debugRenderer->computeDebugRenderingPrimitives(*world);
	const reactphysics3d::DebugRenderer::DebugTriangle* debugTriangles = this->debugRenderer->getTrianglesArray();
	int iterations = this->debugRenderer->getTriangles().size();
	for (int i = 0; i < iterations; i++)
	{
		Vertex newVertex[3];
		newVertex[0].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point1.x, debugTriangles[i].point1.y, debugTriangles[i].point1.z);
		triangles.emplace_back(newVertex[0]);
		newVertex[1].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point2.x, debugTriangles[i].point2.y, debugTriangles[i].point2.z);
		triangles.emplace_back(newVertex[1]);
		newVertex[2].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point3.x, debugTriangles[i].point3.y, debugTriangles[i].point3.z);
		triangles.emplace_back(newVertex[2]);
	}
	this->recreateVertexBuffer();
}

void PhysicsWorld::addSphereToWorld(float radius, DirectX::XMFLOAT3 position)
{
}

//void PhysicsWorld::addSphereToWorld(reactphysics3d::decimal radius, DirectX::XMFLOAT3 position)
//{
//}
