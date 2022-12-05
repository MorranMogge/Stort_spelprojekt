#pragma once
#include "Item.h"

class Potion : public Item
{
private:

public:
	Potion(int componentID = 0);
	~Potion();
	// Inherited via Item
	virtual void use(const Player* playerHoldingItem) override;
};