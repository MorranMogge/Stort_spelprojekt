#include "stdafx.h"
#include "Potion.h"
#include "Player.h"
#include "GUISprite.h"

Potion::Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(useMesh, pos, rot, id, onlineId, 1, field)
{
	//sound
	sfx.load(L"../Sounds/powerUp.wav");


	//Set up Fresnel buffer
	DirectX::SimpleMath::Vector3 ptCol = DirectX::Colors::Green.v;
	fresnelBuffer.Initialize(GPU::device, GPU::immediateContext);
	fresnelBuffer.getData() = DirectX::XMFLOAT4(ptCol.x, ptCol.y, ptCol.z, 1);
	fresnelBuffer.applyData();

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5), 3);
	this->particles->setColor(ptCol);

	//Item Icon
	float constant = 2.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_potion.png", "icon_potion2.png" , "Ekey.png"};
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
}

Potion::Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field)
	:Item(objectPath, pos, rot, id, onlineId, 1, field)
{
	//sound
	sfx.load(L"../Sounds/powerUp.wav");

	//Particles
	this->particles = new ParticleEmitter(pos, rot, 26, DirectX::XMFLOAT2(2, 5),3);
	this->particles->setColor(0,1,0);

	//Set up Fresnel buffer
	fresnelBuffer.Initialize(GPU::device, GPU::immediateContext);
	fresnelBuffer.getData() = DirectX::XMFLOAT4(1, 0.25, 0, 0.5);
	fresnelBuffer.applyData();

	//Item Icon
	float constant = 2.0f;
	DirectX::XMFLOAT3 upDir = this->getUpDirection();
	DirectX::XMFLOAT3 iconPos(upDir.x * constant, upDir.y * constant, upDir.z * constant);
	std::vector<std::string> tempStr{ "icon_potion.png", "icon_potion2.png", "Ekey.png" };
	this->itemIcon = new BilboardObject(tempStr, iconPos);
	this->itemIcon->setOffset(constant);
}

Potion::~Potion()
{
}

void Potion::useItem()
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