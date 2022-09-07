#include "GameObject.h"

GameObject::GameObject(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:position(pos), rotation(rot), mesh(useMesh), objectID(id)
{
	// set position
	mesh->position = pos;

	// set rotation
	mesh->rotation = rot;
}

GameObject::GameObject(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id)
	:position(pos), rotation(rot), objectID(id)
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
	this->mesh->rotation = rot;
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
	this->mesh->rotation = { 0, 0, 0 };

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

void GameObject::setRot(DirectX::XMFLOAT3 rot)
{
	this->rotation = rot;
}

void GameObject::setScale(DirectX::XMFLOAT3 scale)
{
	this->mesh->scale = scale;
}

DirectX::XMFLOAT3 GameObject::getPos() const
{
	return this->position;
}

DirectX::XMFLOAT3 GameObject::getRot() const
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
