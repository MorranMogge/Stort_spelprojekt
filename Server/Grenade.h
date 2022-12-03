#pragma once
#include "Item.h"
#include "TimeStruct.h"

class Grenade : public Item
{
private:
	float force;
	TimeStruct timer;
	bool destructionIsImmenent;

public:
	Grenade(const int& onlineId);
	~Grenade();
	bool explode();
	void resetExplosion();

	virtual void use(const Player* playerHoldingItem) override;
};