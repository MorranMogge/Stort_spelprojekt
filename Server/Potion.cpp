#include "Potion.h"
#include "ObjectId.h"

Potion::Potion(int componentID)
	:Item(componentID, ObjID::POTION)
{
}

Potion::~Potion()
{
}

void Potion::use(const Player* playerHoldingItem)
{
}
