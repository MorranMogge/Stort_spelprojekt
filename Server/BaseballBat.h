#pragma once
#include "Item.h"

#define FORCECONSTANT 40000;



class BaseballBat : public Item
{
private:
	float force;

public:
	BaseballBat(const int& onlineId);
	~BaseballBat();

	//Its not implemented yet************
	// Inherited via Item
	virtual void use(const Player* playerHoldingItem) override;

};
