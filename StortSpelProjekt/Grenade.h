#pragma once
#include "Item.h"
#include "TimeStruct.h"

class Grenade : public Item
{
private:
	std::vector<GameObject*> gameObjects;
	TimeStruct timer;
	bool destructionIsImminent;
	float timeToExplode;

	void explode();
public:
	Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field = nullptr);
	~Grenade();
	void updateExplosionCheck();
	void setGameObjects(const std::vector<GameObject *> &gameObjects);

	// Inherited via Item
	virtual void useItem() override;
};
