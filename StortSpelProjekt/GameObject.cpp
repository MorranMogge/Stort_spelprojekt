#include "PhysicsComponent.h"
#include "GameObject.h"

GameObject::GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:position(pos), rotation(rot), mesh(useMesh), objectID(id), scale(scale)
{
	// set position
	mesh->position = pos;

	// set rotation
	mesh->rotation = rot;

	// set scale
	mesh->scale = scale;
	this->scale = scale;
}

GameObject::GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:position(pos), rotation(rot), objectID(id), scale(scale)
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
	this->mesh->rotation = rot;

	// set scale
	this->mesh->scale = scale;
	this->scale = scale;
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
	this->rotation = rot;
}

void GameObject::setRot(const DirectX::XMVECTOR& rot)
{
	DirectX::XMStoreFloat3(&this->rotation, rot);
}

void GameObject::setScale(DirectX::XMFLOAT3 scale)
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

DirectX::XMFLOAT3 GameObject::getRot() const
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

void GameObject::setMesh(const  std::string& meshPath)
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

void GameObject::update()
{
	this->position = this->physComp->getPosV3();
	this->rotation = DirectX::XMFLOAT3(this->physComp->getRotation().x, this->physComp->getRotation().y, this->physComp->getRotation().z);
}


