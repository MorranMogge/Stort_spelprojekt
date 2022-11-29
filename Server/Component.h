#pragma once
#include <string>
#include "Item.h"
#include "ObjectId.h"

//temp klass eftersom det inte finns en player
class Component : public Item
{
private:


public:
	Component();
	Component(int onlineId, int onlineType = ObjID::COMPONENT);

	virtual void use(const Player* playerHoldingItem) override;
};