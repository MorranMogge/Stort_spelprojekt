#pragma once
#include "Item.h"

#define FORCECONSTANT 4000;

class Player;

class BaseballBat : public Item
{
private:
	Player* player;
	std::vector<GameObject *> objects;

	DirectX::SimpleMath::Vector3 batPos;
	DirectX::SimpleMath::Vector3 savedPos;
	float force;

public:
	BaseballBat(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	BaseballBat(Mesh* mesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	~BaseballBat();

	void setPlayer(Player* player);
	void setTestObj(const std::vector<GameObject *>& objects);
	// Inherited via Item
	virtual void useItem() override;

};
