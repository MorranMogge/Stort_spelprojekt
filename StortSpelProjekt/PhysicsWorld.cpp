#include "PhysicsWorld.h"
#include "GPU.h"
#include "ShaderLoader.h"

void PhysicsWorld::setUpBaseScenario()
{
	//Create Player
	playerShape = com.createBoxShape(reactphysics3d::Vector3(5, 5, 5));
	reactphysics3d::Transform playerTransform = reactphysics3d::Transform(reactphysics3d::Vector3(10, 10, -10), reactphysics3d::Quaternion::identity());
	playerRigidBody = world->createRigidBody(playerTransform);
	playerCollider = playerRigidBody->addCollider(playerShape, reactphysics3d::Transform(reactphysics3d::Vector3(0, 0, 0), reactphysics3d::Quaternion::identity()));
	playerRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);
	playerRigidBody->enableGravity(false);
	playerRigidBody->setMass(1000);
	playerRigidBody->applyLocalTorque(reactphysics3d::Vector3(10000, 10000, 10000));


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
	//debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
	//debugRenderer->setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
	debugRenderer->computeDebugRenderingPrimitives(*world);
}

void PhysicsWorld::updateVertexBuffer()
{
	//debugRenderer->computeDebugRenderingPrimitives(*world);
	const reactphysics3d::DebugRenderer::DebugTriangle* debugTriangles = this->debugRenderer->getTrianglesArray();
	if (this->debugRenderer->getTriangles().size() * 3 > 1008)
	{
		int test = 0;
	}
	for (int i = 0; i < this->debugRenderer->getTriangles().size(); i++)
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
		triangles.push_back(newVertex[0]);
		newVertex[1].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point2.x, debugTriangles[i].point2.y, debugTriangles[i].point2.z);
		triangles.push_back(newVertex[1]);
		newVertex[2].position = DirectX::SimpleMath::Vector3(debugTriangles[i].point3.x, debugTriangles[i].point3.y, debugTriangles[i].point3.z);
		triangles.push_back(newVertex[2]);
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
}

void PhysicsWorld::renderReact3D()
{
	this->updateVertexBuffer();
	GPU::immediateContext->VSSetConstantBuffers(0, 1, &identityMatrix);
	GPU::immediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	GPU::immediateContext->IASetVertexBuffers(0, 1, &debuggerBuffer, &stride, &offset);
	//GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GPU::immediateContext->PSSetShader(dpShader, nullptr, 0);
	GPU::immediateContext->Draw(this->triangles.size(), 0);
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
}

void PhysicsWorld::update(float dt)
{
	world->update(dt);
}

void PhysicsWorld::addForceToObject(const DirectX::XMFLOAT3& gravityVec)
{
	playerRigidBody->applyLocalForceAtCenterOfMass(playerRigidBody->getMass() * reactphysics3d::Vector3(gravityVec.x, gravityVec.y, gravityVec.z));
}

DirectX::SimpleMath::Vector3 PhysicsWorld::getPos()
{
	return { playerRigidBody->getTransform().getPosition().x, playerRigidBody->getTransform().getPosition().y , playerRigidBody->getTransform().getPosition().z };
}

DirectX::SimpleMath::Vector3 PhysicsWorld::getRot()
{
	return { playerRigidBody->getTransform().getOrientation().x, playerRigidBody->getTransform().getOrientation().y, playerRigidBody->getTransform().getOrientation().z };
}