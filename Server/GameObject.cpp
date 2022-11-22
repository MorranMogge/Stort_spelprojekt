#include "PhysicsComponent.h"
#include "GameObject.h"


void GameObject::updatePhysCompRotation()
{
	DirectX::SimpleMath::Quaternion dx11Quaternion = DirectX::XMQuaternionRotationMatrix(this->rotation);
	reactphysics3d::Quaternion reactQuaternion = reactphysics3d::Quaternion(dx11Quaternion.x, dx11Quaternion.y, dx11Quaternion.z, dx11Quaternion.w);
	this->physComp->setRotation(reactQuaternion);
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(dx11Quaternion.ToEuler());
}

GameObject::GameObject( const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:position(pos), objectID(id), scale(scale), physComp(nullptr)
{

	// set rotation
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));

	// set scale
	this->scale = scale;
}

GameObject::GameObject()
{
	this->rotation = DirectX::XMMatrixIdentity();
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
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
}

void GameObject::setRot(const DirectX::XMVECTOR& rot)
{
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(rot);
}

void GameObject::setScale(const DirectX::XMFLOAT3& scale)
{
	this->scale = scale;

	//if (this->physComp->getTypeName() == reactphysics3d::CollisionShapeName::BOX) 
	this->physComp->setScale(scale);
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
		* this->rotation * DirectX::XMMatrixTranslation(this->position.x, this->position.y, this->position.z)) }));
	return temp;
}

void GameObject::setMatrix(DirectX::XMFLOAT4X4 matrix)
{
	this->position.x = matrix._14;
	this->position.y = matrix._24;
	this->position.z = matrix._34;
	this->physComp->setPosition(reactphysics3d::Vector3{ position.x,position.y, position.z });
}

void GameObject::setPhysComp(PhysicsComponent* comp)
{
	this->physComp = comp;
}

PhysicsComponent* GameObject::getPhysComp() const
{
	return this->physComp;
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
void GameObject::updateRotation()
{
	/*this->position = this->physComp->getPosV3();
	this->reactQuaternion = this->physComp->getRotation();
	this->dx11Quaternion = DirectX::SimpleMath::Quaternion(DirectX::SimpleMath::Vector4(reactQuaternion.x, reactQuaternion.y, reactQuaternion.z, reactQuaternion.w));*/
}

int GameObject::getId()
{
	return this->objectID;
}

void GameObject::update()
{
	this->updatePhysCompRotation();
}