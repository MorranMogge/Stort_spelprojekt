#pragma once
#include "Item.h"

class Grenade : public Item
{
private:
	float force;

public:
	Grenade(const int& onlineId);
	~Grenade();

	virtual void use(const Player* playerHoldingItem) override;

};