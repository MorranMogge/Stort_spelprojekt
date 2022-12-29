#pragma once
#include "Item.h"

#define FORCECONSTANT 40000;

class BaseballBat : public Item
{
private:
	//Player* playa;

	//DirectX::SimpleMath::Vector3 savedPos;
	float force;

	//void sendForceToServer(const DirectX::SimpleMath::Vector3& hitForce, const int& playerID);
public:
	//BaseballBat(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId);
	BaseballBat(const int& onlineId);
	~BaseballBat();

	//void setPlayer(Player* player);
	//void setGameObjects(const std::vector<GameObject*>& objects);
	//void setGameObjects(const std::vector<Player*>& objects);


	//Its not implemented yet************
	// Inherited via Item
	virtual void use(const Player* playerHoldingItem) override;
};