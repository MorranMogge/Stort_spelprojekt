#include "GameObject.h"

GameObject::GameObject(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:position(pos), mesh(useMesh), objectID(id)
{
	// set position
	mesh->position = pos;

	// set rotation
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->mesh->rotation = this->rotation;
}

GameObject::GameObject(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:position(pos), objectID(id)
{
	// load obj file
	OBJ testObj(objectPath);
	this->mesh = new Mesh(testObj);


	// load all materials for Obj
	int nrOfMat = testObj.mtl.materials.size();
	for (int i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}
	
	// set position
	this->mesh->position = pos;

	// set rotation
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->mesh->rotation = this->rotation;
}

GameObject::GameObject()
{
	// load obj file
	OBJ testObj("../Meshes/pinto");
	this->mesh = new Mesh(testObj);


	// load all materials for Obj
	int nrOfMat = testObj.mtl.materials.size();
	for (int i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}

	// set position
	this->mesh->position = { 0, 0, 0 };

	// set rotation
	this->mesh->rotation = DirectX::XMMatrixIdentity();
	this->rotation = DirectX::XMMatrixIdentity();

	//Update constantbuffer
}

GameObject::~GameObject()
{
	if (this->mesh != nullptr)
	{
		delete this->mesh;
	}
}

void GameObject::setPos(DirectX::XMFLOAT3 pos)
{
	this->position = pos;
}

void GameObject::setRot(DirectX::XMMATRIX rot)
{
	this->mesh->rotation = rot;
	this->rotation = rot;
}

void GameObject::setRot(DirectX::XMFLOAT3 rot)
{
	this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
}

void GameObject::setScale(DirectX::XMFLOAT3 scale)
{
	this->mesh->scale = scale;
}

DirectX::XMFLOAT3 GameObject::getPos() const
{
	return this->position;
}

DirectX::XMMATRIX GameObject::getRot() const
{
	return this->rotation;
}

Bound* GameObject::getBounds() const
{
	return &this->mesh->bound; //funkar??
}

void GameObject::updateBuffer()
{
	//Set mesh pos & rot to current member variable pos/rot
	this->mesh->position = this->position;
	this->mesh->rotation = this->rotation;

	//Update constantbuffer
	this->mesh->UpdateCB();
}

void GameObject::setMesh(std::string meshPath)
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
	int nrOfMat = testObj.mtl.materials.size();
	for (int i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}

	// set position
	this->mesh->position = this->position;

	// set rotation
	this->mesh->rotation = this->rotation;
}

void GameObject::setMesh(Mesh*)
{

	//delete current mesh ptr
	if (this->mesh != nullptr)
	{
		delete this->mesh;
	}


	// set position
	this->mesh->position = this->position;

	// set rotation
	this->mesh->rotation = this->rotation;
}

bool GameObject::withinRadious(GameObject* object, float radius) const
{
	using namespace DirectX;

	XMFLOAT3 objPos = object->getPos();
	XMFLOAT3 selfPos = this->getPos();
	bool inRange = false;

	//X range
	if (objPos.x <= selfPos.x + radius && objPos.x >= selfPos.x - radius)
	{
		//Y range
		if (objPos.y <= selfPos.y + radius && objPos.y >= selfPos.y - radius)
		{
			//Z range
			if (objPos.z <= selfPos.z + radius && objPos.z >= selfPos.z - radius)
			{
				inRange = true;
			}
		}
	}

	return inRange;
}

void GameObject::draw()
{
	this->mesh->DrawWithMat();
}


