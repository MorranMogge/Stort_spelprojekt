#include "stdafx.h"
#include "Potion.h"
#include "Player.h"

Potion::Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(useMesh, pos, rot, id, onlineId, 1, field)
{
	sfx.load(L"../Sounds/powerUp.wav");
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 3);

	//Item Icon
	float constant = 2.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_potion.png", "icon_potion2.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
}

Potion::Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(objectPath, pos, rot, id, onlineId, 1, field)
{
	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5),3);
	
	//Item Icon
	float constant = 2.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_potion.png", "icon_potion2.png"};
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
}

Potion::~Potion()
{
}

void Potion::useItem(const Player* playerHoldingItem)
{
	sfx.stop();
	sfx.play();
	this->setPos({ -100, -100, -100 });
	timeToRun = true;
	timer.resetStartTime();
}

bool Potion::isTimeToRun() const
{
	return this->timeToRun;
}

bool Potion::timerGoing()
{
	if (this->timer.getTimePassed(5.f)) timeToRun = false;
	return timeToRun;
}