#include "stdafx.h"
#include "PhysicsWorld.h"

#include <time.h>
#include "GPU.h"
#include "ShaderLoader.h"
#include "DirectXMathHelper.h"

#include "GameObject.h"
#include "PhysicsComponent.h"

void PhysicsWorld::setUpBaseScenario()
{
	playerBox = new PhysicsComponent();
	playerBox->initiateComponent(&this->com, this->world);
	playerBox->setType(reactphysics3d::BodyType::KINEMATIC);

	world->setIsDebugRenderingEnabled(true);
	debugRenderer = &world->getDebugRenderer();
	debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	//debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);

	debugRenderer->computeDebugRenderingPrimitives(*world);
}

void PhysicsWorld::updateVertexBuffer()
{
	//debugRenderer->computeDebugRenderingPrimitives(*world);
	const reactphysics3d::DebugRenderer::DebugTriangle* debugTriangles = this->debugRenderer->getTrianglesArray(); //If save points (Vector3) as pointer maybe you do not have to copy every frame
	for (int i = 0; i < this->debugRenderer->getTriangles().size(); i++) //We copy the triangle data every frame
	{
		int first	=	3 * i + 0;
		int second	=	3 * i + 1;
		int third	=	3 * i + 2;


		this->triangles[first].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point1.x, debugTriangles[i].point1.y, debugTriangles[i].point1.z);
		this->triangles[second].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point2.x, debugTriangles[i].point2.y, debugTriangles[i].point2.z);
		this->triangles[third].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point3.x, debugTriangles[i].point3.y, debugTriangles[i].point3.z);
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	GPU::immediateContext->Map(debuggerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, this->triangles.data(), this->triangles.size()*sizeof(Vertex));
	GPU::immediateContext->Unmap(debuggerBuffer, 0);
}

bool PhysicsWorld::recreateVertexBuffer()
{
	this->triangles.clear();
	this->debugRenderer->reset();
	this->debugRenderer->computeDebugRenderingPrimitives(*world);
	const reactphysics3d::DebugRenderer::DebugTriangle* debugTriangles = this->debugRenderer->getTrianglesArray();
	reactphysics3d::uint64 iterations = this->debugRenderer->getTriangles().size();
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

	debuggerBuffer->Release();

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = UINT(sizeof(Vertex) * reactphysics3d::uint64(this->triangles.size()));
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
	D3D11_RASTERIZER_DESC wireframedesc = {};
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
	bufferDesc.ByteWidth = UINT(sizeof(Vertex) * reactphysics3d::uint64(this->triangles.size()));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

	return !FAILED(hr);
}

PhysicsWorld::PhysicsWorld(std::string worldName)
{
	// Create the world settings 
	reactphysics3d::PhysicsWorld::WorldSettings settings;
	settings.defaultVelocitySolverNbIterations = 20;
	settings.isSleepingEnabled = false;
	settings.worldName = worldName;
	settings.isSleepingEnabled = true;
	settings.gravity = reactphysics3d::Vector3(0, 0, 0);
	settings.defaultBounciness = 0.1f;
	settings.defaultFrictionCoefficient = 0.1f;
	settings.defaultTimeBeforeSleep = 1.f;

	// Create the physics world with your settings 
	world = com.createPhysicsWorld(settings);
	this->setUpBaseScenario();
	this->setVertexBuffer();
	this->setUpWireframe();
}

void PhysicsWorld::renderReact3D()
{
	nrOfTriangles = this->triangles.size();
	this->debugRenderer->reset();
	this->debugRenderer->computeDebugRenderingPrimitives(*world);
	if (3*nrOfTriangles != this->debugRenderer->getTriangles().size()) this->recreateVertexBuffer();
	else this->updateVertexBuffer();
	
	GPU::immediateContext->VSSetConstantBuffers(0, 1, &identityMatrix);
	GPU::immediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	GPU::immediateContext->IASetVertexBuffers(0, 1, &debuggerBuffer, &stride, &offset);
	GPU::immediateContext->RSSetState(wireframeMode);
	GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GPU::immediateContext->PSSetShader(dpShader, nullptr, 0);
	GPU::immediateContext->Draw(UINT(reactphysics3d::uint64(this->triangles.size())), 0);
	GPU::immediateContext->RSSetState(nullptr);
}

PhysicsWorld::~PhysicsWorld()
{
	for (int i = 0; i < physObjects.size(); i++)
	{
		delete physObjects[i];
	}
	delete playerBox;
	if (world != nullptr) com.destroyPhysicsWorld(world);
	debuggerBuffer->Release();
	dpShader->Release();
	identityMatrix->Release();
	wireframeMode->Release();
}

void PhysicsWorld::setPlanets(std::vector<Planet*>& planets)
{
	this->planets = planets;
}

void PhysicsWorld::update(const float& dt)
{
	this->addForceToObjects(dt);
	world->update(dt);
}

void PhysicsWorld::addForceToObjects(const float& dt)
{
	float constant = 1.f;
	for (int i = 0; i < this->physObjects.size(); i++)
	{
		grav = planets[0]->getClosestFieldFactor(planets, this->physObjects[i]->getPosV3());;
		this->physObjects[i]->applyForceToCenter(this->physObjects[i]->getMass() * reactphysics3d::Vector3(98.2f * grav.x * dt * constant, 98.2f * grav.y * dt * constant, 98.2f * grav.z * dt * constant));
	}
}

PhysicsComponent* PhysicsWorld::getPlayerBox() const
{
	return this->playerBox;
}

void PhysicsWorld::addBoxToWorld(DirectX::XMFLOAT3 dimensions, float mass, DirectX::XMFLOAT3 position)
{
	if (position.x == 0 && position.y == 10 && position.z == 0)
	{
		float x = 50.f - 2.f * (float)(rand() % 61);
		float y = 50.f - 2.f * (float)(rand() % 61);
		float z = 50.f - 2.f * (float)(rand() % 61);
		position = DirectX::XMFLOAT3(x, y, z);
	}
	UINT vectorSize = UINT(this->physObjects.size());
	physObjects.emplace_back(new PhysicsComponent());
	physObjects[vectorSize]->initiateComponent(&this->com, this->world);
	physObjects[vectorSize]->setPosition(reactphysics3d::Vector3(position.x, position.y, position.z));
	physObjects[vectorSize]->setLinearDampning(1.3f);
	//UPDATE THE VERTEX BUFFER TO BE ABLE TO DRAW THE NEW PHYSOBJECTS
	this->recreateVertexBuffer();
}

void PhysicsWorld::addPhysComponent(GameObject* gameObj, reactphysics3d::CollisionShapeName shape, const DirectX::XMFLOAT3& scale)
{
	PhysicsComponent* newComp = new PhysicsComponent();
	newComp->initiateComponent(&this->com, this->world, shape, scale);
	newComp->setPosition({ gameObj->getPos().x, gameObj->getPos().y, gameObj->getPos().z });
	newComp->setLinearDampning(0.3f);
	gameObj->setPhysComp(newComp);
	newComp->setParent(gameObj);
	physObjects.emplace_back(newComp);

	this->recreateVertexBuffer();
}

PhysicsComponent* PhysicsWorld::returnAddedPhysComponent(reactphysics3d::CollisionShapeName shape, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& scale)
{
	PhysicsComponent* newComp = new PhysicsComponent();
	newComp->initiateComponent(&this->com, this->world, shape, scale);
	newComp->setPosition({ pos.x, pos.y, pos.z });
	newComp->setLinearDampning(0.3f);
	physObjects.emplace_back(newComp);

	this->recreateVertexBuffer();
	return newComp;
}

