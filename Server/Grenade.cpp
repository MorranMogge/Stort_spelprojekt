#include "Grenade.h"
#include "ObjectId.h"

Grenade::Grenade(const int& onlineId)
	:Item(onlineId, ObjID::GRENADE), force(0.f)
{
	this->destructionIsImmenent = false;
}

Grenade::~Grenade()
{

}

bool Grenade::explode()
{
	return (this->destructionIsImmenent && this->timer.getTimePassed(5.f));
}

void Grenade::resetExplosion()
{
	this->destructionIsImmenent = false;
}

void Grenade::use(const Player* playerHoldingItem)
{
	std::cout << "Used grenade\n";
	timer.resetStartTime();
}
