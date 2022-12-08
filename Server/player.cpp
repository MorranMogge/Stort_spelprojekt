
#include "Player.h"

Player::Player()
{
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
	
	readyStatus = -1;
	this->onlineId = -1;
	this->animID = 0;
	this->animSpeed = 1;
	this->dead = false;
}

float Player::getposition(char whichPos) const
{
	switch (whichPos)
	{
	case('x'):
		return this->physComp->getPosition().x;
		break;
	case('y'):
		return this->physComp->getPosition().y;
		break;
	case('z'):
		return this->physComp->getPosition().z;
		break;
	default:
		return 69.0f;
		break;
	}
}

void Player::playerGotHit(const reactphysics3d::Vector3& force)
{
	this->dead = true;
	this->physComp->setType(reactphysics3d::BodyType::DYNAMIC);
	this->physComp->applyForceToCenter(force);
	this->physComp->applyWorldTorque(force);
	timer.resetStartTime();
}

bool Player::getDeathState()
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

void Player::setPhysicsComponent(PhysicsComponent* physComp)
{
	this->physComp = physComp;
	this->physComp->setType(reactphysics3d::BodyType::KINEMATIC);
}

PhysicsComponent* Player::getPhysComp() const
{
	return this->physComp;
}

void Player::setMatrix(DirectX::XMFLOAT4X4 matrix)
{
	this->matrix = matrix;
	DirectX::SimpleMath::Quaternion quat;
	quat = DirectX::XMQuaternionRotationMatrix(DirectX::XMLoadFloat4x4(&matrix));
	this->physComp->setPosition(reactphysics3d::Vector3(matrix._14, matrix._24, matrix._34));
	this->physComp->setRotation(reactphysics3d::Quaternion(quat.x, quat.y, quat.z, quat.w));
}

void Player::setOnlineId(int nr)
{
	onlineId = nr;
}

void Player::setReadyStatus(int status)
{
	readyStatus = status;
}

int Player::getReadyStatus()const
{
	return readyStatus;
}

int Player::getOnlineID()const
{
	return onlineId;
}

DirectX::XMFLOAT4X4 Player::getMatrix() const
{ 
	return this->matrix;
}

void Player::setPosition(float pos[3])
{
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
	this->physComp->setPosition(reactphysics3d::Vector3(pos[0], pos[1], pos[2]));
}

void Player::setPosition(float x, float y, float z)
{
	this->matrix._14 = x;
	this->matrix._24 = y;
	this->matrix._34 = z;
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
	this->physComp->setPosition(reactphysics3d::Vector3(x, y, z));
}

void Player::updatePosViaPhysComp()
{
	reactphysics3d::Quaternion reactQuat = this->physComp->getRotation();
	DirectX::SimpleMath::Quaternion quat(reactQuat.x, reactQuat.y, reactQuat.z, reactQuat.w);
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixRotationRollPitchYawFromVector(quat.ToEuler()));
	this->matrix._14 = xPos;
	this->matrix._24 = yPos;
	this->matrix._34 = zPos;
}

void Player::setAnimData(int animId, float animSpeed)
{
	this->animID = animId;
	this->animSpeed = animSpeed;
}

void Player::getAnimData(int& animId, float& animSpeed)
{
	animId = this->animID;
	animSpeed = this->animSpeed;
}