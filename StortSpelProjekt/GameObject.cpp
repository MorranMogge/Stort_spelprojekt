#include "stdafx.h"
#include "PhysicsComponent.h"
#include "GameObject.h"

GameObject::GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:position(pos), mesh(useMesh), objectID(id), scale(scale), physComp(nullptr)
{


	

	// set position
	mesh->position = pos;

	// set rotation
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->mesh->rotation = this->rotation;

	// set scale
	mesh->scale = scale;
	this->scale = scale;
}

GameObject::GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:position(pos), objectID(id), scale(scale), physComp(nullptr)
{
	// load obj file
	OBJ testObj(meshPath);
	this->mesh = new Mesh(testObj);


	// load all materials for Obj
	int nrOfMat = (int)testObj.mtl.materials.size();
	for (int i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}
	
	// set position
	this->mesh->position = pos;

	// set rotation
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->mesh->rotation = this->rotation;

	// set scale
	this->mesh->scale = scale;
	this->scale = scale;

	this->mesh->UpdateCB();
}

GameObject::GameObject()
{
	// load obj file
	OBJ testObj("../Meshes/pinto");
	this->mesh = new Mesh(testObj);


	// load all materials for Obj

	UINT nrOfMat = UINT(testObj.mtl.materials.size());
	for (UINT i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}

	// set position
	this->mesh->position = { 0, 0, 0 };

	// set rotation
	this->mesh->rotation = DirectX::XMMatrixIdentity();
	this->rotation = DirectX::XMMatrixIdentity();

	//Update constantbuffer
	this->updateBuffer();
}

GameObject::~GameObject()
{
	if (this->mesh != nullptr)
	{
		delete this->mesh;
	}
}

void GameObject::movePos(const DirectX::XMFLOAT3& offset)
{
	this->position.x += offset.x;
	this->position.y += offset.y;
	this->position.z += offset.z;
}

void GameObject::setPos(const DirectX::XMFLOAT3& pos)
{
	this->position = pos;
}

void GameObject::setRot(const DirectX::XMFLOAT3& rot)
{
	this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
}

void GameObject::setRot(const DirectX::XMVECTOR& rot)
{
	this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
}

void GameObject::setScale(const DirectX::XMFLOAT3& scale)
{
	this->mesh->scale = scale;
	this->scale = scale;
}

DirectX::XMFLOAT3 GameObject::getPos() const
{
	return this->position;
}

DirectX::SimpleMath::Vector3 GameObject::getPosV3() const
{
	return this->position;
}

DirectX::XMMATRIX GameObject::getRot() const
{
	return this->rotation;
}

DirectX::XMFLOAT3 GameObject::getScale() const
{
	return this->scale;
}

Bound* GameObject::getBounds() const
{
	return &this->mesh->bound; //funkar??
}

void GameObject::setPhysComp(PhysicsComponent* comp)
{
	this->physComp = comp;
}

PhysicsComponent* GameObject::getPhysComp() const
{
	return this->physComp;
}

void GameObject::updateBuffer()
{
	//Set mesh pos & rot to current member variable pos/rot
	this->mesh->position = this->position;
	this->mesh->rotation = this->rotation;
	this->mesh->scale = this->scale;

	//Update constantbuffer
	this->mesh->UpdateCB();
}

void GameObject::setMesh(const std::string& meshPath)
{
	//delete current mesh ptr
	if (this->mesh != nullptr)
	{
		delete this->mesh;
	}

	// load obj file
	OBJ testObj(meshPath);
	this->mesh = new Mesh(testObj);


	// load all materials for Obj
	UINT nrOfMat = UINT(testObj.mtl.materials.size());
	for (UINT i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}

	// set position
	this->mesh->position = this->position;

	// set rotation
	this->mesh->rotation = this->rotation;
}


void GameObject::setMesh(Mesh* inMesh)
{

	//delete current mesh ptr
	if (this->mesh != nullptr)
	{
		delete this->mesh;
	}

	this->mesh = inMesh;

	// set position
	this->mesh->position = inMesh->position;

	// set rotation
	this->mesh->rotation = inMesh->rotation;
}

bool GameObject::withinBox(GameObject* object, float xRange, float yRange, float zRange) const
{
	using namespace DirectX;

	XMFLOAT3 objPos = object->getPos();
	XMFLOAT3 selfPos = this->getPos();
	bool inRange = false;

	//X range
	if (objPos.x <= selfPos.x + xRange && objPos.x >= selfPos.x - xRange)
	{
		//Y range
		if (objPos.y <= selfPos.y + yRange && objPos.y >= selfPos.y - yRange)
		{
			//Z range
			if (objPos.z <= selfPos.z + zRange && objPos.z >= selfPos.z - zRange)
			{
				inRange = true;
			}
		}
	}

	return inRange;
}

bool GameObject::withinRadious(GameObject* object, float radius) const
{
	using namespace DirectX;

	XMFLOAT3 objPos = object->getPos();
	XMFLOAT3 selfPos = this->position;
	bool inRange = false;

	float x = (selfPos.x - objPos.x) * (selfPos.x - objPos.x);
	float y = (selfPos.y - objPos.y) * (selfPos.y - objPos.y);
	float z = (selfPos.z - objPos.z) * (selfPos.z - objPos.z);

	float sum = std::sqrt(x + y + z);

	//DirectX::SimpleMath::Vector3 vector(x, y, z);
	if (abs(sum) < radius)
	{
		inRange = true;
	}

	return inRange;
}

void GameObject::draw()
{
	this->mesh->DrawWithMat();
}

int GameObject::getId()
{
	return this->objectID;
}

void GameObject::update()
{
	this->position = this->physComp->getPosV3();
	//this->rotation = DirectX::XMFLOAT3(this->physComp->getRotation().x, this->physComp->getRotation().y, this->physComp->getRotation().z);
	this->reactQuaternion = this->physComp->getRotation();
	this->dx11Quaternion = DirectX::SimpleMath::Quaternion(DirectX::SimpleMath::Vector4(reactQuaternion.x, reactQuaternion.y, reactQuaternion.z, reactQuaternion.w));
	this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(dx11Quaternion.ToEuler());
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(dx11Quaternion.ToEuler());
}

void GameObject::tmpDraw()
{
	this->mesh->draw(this->srv);
}

void GameObject::setSrv(ID3D11ShaderResourceView* srv)
{
	this->srv = srv;
}