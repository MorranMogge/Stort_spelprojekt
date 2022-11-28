#include "Grenade.h"
#include "ObjectId.h"

Grenade::Grenade(const int& onlineId)
	:Item(onlineId, ObjID::GRENADE), force(0.f)
{

}

Grenade::~Grenade()
{

}

void Grenade::use(const Player* playerHoldingItem)
{
	std::cout << "Used grenade\n";
}
