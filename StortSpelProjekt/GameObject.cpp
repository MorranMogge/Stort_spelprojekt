#include "stdafx.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include "DirectXMathHelper.h"

void GameObject::updatePhysCompRotation()
{
	DirectX::SimpleMath::Quaternion dx11Quaternion = DirectX::XMQuaternionRotationMatrix(this->rotation);
	reactphysics3d::Quaternion reactQuaternion = reactphysics3d::Quaternion(dx11Quaternion.x, dx11Quaternion.y, dx11Quaternion.z, dx11Quaternion.w);
	this->physComp->setRotation(reactQuaternion);
}

GameObject::GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field, const DirectX::XMFLOAT3& scale)
	:position(pos), objectID(id), scale(scale), physComp(nullptr), activeField(field)
{

	this->mesh = useMesh;

	// set position
	mesh->position = pos;

	// set rotation
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
	this->mesh->rotation = this->rotation;

	// set scale
	mesh->scale = scale;
	this->scale = scale;

}

GameObject::GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field, const DirectX::XMFLOAT3& scale)
	:position(pos), objectID(id), scale(scale), physComp(nullptr), activeField(field)
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
	//if (this->mesh != nullptr)
	//{
	//	delete this->mesh;
	//}
	//this->mesh = nullptr;
	//this->activeField = nullptr;
	//this->physComp = nullptr;
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
	this->physComp->setPosition(reactphysics3d::Vector3{ pos.x,pos.y, pos.z });

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
	
	//if (this->physComp->getTypeName() == reactphysics3d::CollisionShapeName::BOX) 
	//this->physComp->setScale(scale);
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

DirectX::XMFLOAT3 GameObject::getRotXM() const
{
	return DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(this->rotation).ToEuler();
}

DirectX::XMFLOAT3 GameObject::getScale() const
{
	return this->scale;
}

DirectX::XMFLOAT4X4 GameObject::getMatrix() const
{
	DirectX::XMFLOAT4X4 temp;
	DirectX::XMStoreFloat4x4(&temp, DirectX::XMMatrixTranspose({ (DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
		* this->rotation * DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z))}));
	return temp;
}

void GameObject::setMatrix(DirectX::XMFLOAT4X4 matrix)
{
	this->mesh->setMatrix(matrix);
	this->position.x = matrix._14;
	this->position.y = matrix._24;
	this->position.z = matrix._34;
	this->physComp->setPosition(reactphysics3d::Vector3{ position.x,position.y, position.z });
}

void GameObject::updateMatrixOnline()
{
	this->mesh->updateONLINE();
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

void GameObject::orientToUpDirection()
{
	using namespace DirectX::SimpleMath;
	if (this->activeField != nullptr)
	{
		Vector3 yAxis(this->activeField->calcGravFactor(this->position) * -1);
		Vector3 zAxis = yAxis.Cross({ 0, 0, 1 });
		zAxis.Normalize();

		Vector3 xAxis = yAxis.Cross(zAxis);
		xAxis.Normalize();
		this->rotation = Matrix(xAxis, yAxis, zAxis);
	}
	else
	{
		std::cout << "Gravity field was nullptr, rotation was not changed" << std::endl;
	}
}

DirectX::XMFLOAT3 GameObject::getUpDirection() const
{
	DirectX::XMFLOAT3 upDir(0, 0, 0);
	if (this->activeField != nullptr)
	{
		using namespace DirectX::SimpleMath;
		Vector3 yAxis(this->activeField->calcGravFactor(this->position) * -1);
		upDir = yAxis;
	}
	else
	{
		std::cout << "Gravity field was nullptr, direction was not given" << std::endl;
	}
	newNormalizeXMFLOAT3(upDir);
	return upDir;
}

DirectX::XMFLOAT3 GameObject::getRotOrientedToGrav() const
{
	using namespace DirectX::SimpleMath;

	DirectX::XMFLOAT3 finalRot(0,0,0);

	if (this->activeField != nullptr)
	{
		Vector3 yAxis( this->activeField->calcGravFactor(this->position) * -1);
		finalRot = Quaternion::LookRotation({ 0, 0, -1 }, yAxis).ToEuler();
	}
	else
	{
		std::cout << "Gravity field was nullptr, rotation was not given" << std::endl;
	}

	return finalRot;

}


void GameObject::updateBuffer()
{
	//Set mesh pos & rot to current member variable pos/rot
	this->mesh->position = this->position;
	this->mesh->rotation = this->rotation;
	this->mesh->scale = this->scale;

	//Update constantbuffer
	//this->mesh->UpdateCB();
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
	this->mesh->UpdateCB(position, rotation, scale);
	this->mesh->DrawWithMat();
}

void GameObject::updateRotation()
{
	this->position = this->physComp->getPosV3();
	this->reactQuaternion = this->physComp->getRotation();
	this->dx11Quaternion = DirectX::SimpleMath::Quaternion(DirectX::SimpleMath::Vector4(reactQuaternion.x, reactQuaternion.y, reactQuaternion.z, reactQuaternion.w));
	this->mesh->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(dx11Quaternion.ToEuler());
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(dx11Quaternion.ToEuler());
}

int GameObject::getId()
{
	return this->objectID;
}

void GameObject::update()
{
	this->updateRotation();
}