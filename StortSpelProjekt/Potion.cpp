#include "stdafx.h"
#include "Potion.h"
#include "Player.h"

Potion::Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(useMesh, pos, rot, id)
{
	
}

Potion::Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:Item(objectPath, pos, rot, id)
{

}

Potion::~Potion()
{
}

void Potion::useItem()
{
	this->setPos({ -100, -100, -100 });
	timeToRun = true;
	timer.resetStartTime();
}

bool Potion::isTimeToRun()
{
	if (timeToRun && this->timer.getTimePassed(5.f)) timeToRun = false;
	return this->timeToRun;
}