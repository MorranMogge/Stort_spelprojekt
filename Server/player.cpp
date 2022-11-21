
#include "player.h"

player::player()
{
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
	this->dead = false;
}

float player::getposition(char whichPos) const
{
	switch (whichPos)
	{
	case('x'):
		return this->physComp->getPosition().x;;//oiasfjaoiwfjoiawfjoiawfjoiawfjoiawfjoiawfjoiawfjaoiwfjoawifjafwoij
		break;
	case('y'):
		return this->physComp->getPosition().y;;
		break;
	case('z'):
		return this->physComp->getPosition().z;;
		break;
	default:
		return 69.0f;
		break;
	}
}

void player::playerGotHit(const reactphysics3d::Vector3& force)
{
	this->dead = true;
	this->physComp->setType(reactphysics3d::BodyType::DYNAMIC);
	this->physComp->applyForceToCenter(force);
	this->physComp->applyWorldTorque(force);
	timer.resetStartTime();
}

bool player::getDeathState()
{
	if (dead && timer.getTimePassed(5.0f))
	{
		this->dead = false;
		this->physComp->resetForce();
		this->physComp->resetTorque();
		this->physComp->setType(reactphysics3d::BodyType::STATIC);
		this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
	}
	return this->dead;
}

void player::setPhysicsComponent(PhysicsComponent* physComp)
{
	this->physComp = physComp;
	this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
}

PhysicsComponent* player::getPhysComp() const
{
	return this->physComp;
}

void player::setMatrix(DirectX::XMFLOAT4X4 matrix)
{
	this->matrix = matrix;
	DirectX::SimpleMath::Quaternion quat;
	quat = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&matrix));
	this->physComp->setRotation(reactphysics3d::Quaternion(quat.x, quat.y, quat.z, quat.w));
}

DirectX::XMFLOAT4X4 player::getMatrix() const
{ 
	return this->matrix;
}

void player::setPosition(float pos[3])
{
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
	this->physComp->setPosition(reactphysics3d::Vector3(pos[0], pos[1], pos[2]));
}

void player::setPosition(float x, float y, float z)
{
	this->matrix._14 = x;
	this->matrix._24 = y;
	this->matrix._34 = z;
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
	this->physComp->setPosition(reactphysics3d::Vector3(x, y, z));
}

void player::updatePosViaPhysComp()
{
	reactphysics3d::Quaternion reactQuat = this->physComp->getRotation();
	DirectX::SimpleMath::Quaternion quat(reactQuat.x, reactQuat.y, reactQuat.z, reactQuat.w);
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixRotationRollPitchYawFromVector(quat.ToEuler()));
	this->matrix._14 = xPos;
	this->matrix._24 = yPos;
	this->matrix._34 = zPos;
}