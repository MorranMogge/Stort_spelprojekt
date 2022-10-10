#pragma once
#include "Item.h"

#define FORCECONSTANT 4000;

class Player;

class BaseballBat : public Item
{
private:
	Player* player;
	GameObject* testObj;
	std::vector<GameObject *> objects;

public:
	BaseballBat(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	~BaseballBat();

	void setPlayer(Player* player);
	void setTestObj(const std::vector<GameObject *>& objects);
	// Inherited via Item
	virtual void useItem() override;

};
