#pragma once
#include "Item.h"

#define FORCECONSTANT 40000;

class Player;
class Client;

class BaseballBat : public Item
{
private:
	Player* player;
	Client* client;
	std::vector<GameObject*> objects;

	DirectX::SimpleMath::Vector3 batPos;
	DirectX::SimpleMath::Vector3 savedPos;
	float force;

	void sendForceToServer(const DirectX::SimpleMath::Vector3& hitForce, const int& playerID);
public:
	BaseballBat(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	BaseballBat(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	~BaseballBat();

	void setPlayer(Player* player);
	void setClient(Client* client);
	void setTestObj(const std::vector<GameObject*>& objects);
	void setGameObjects(const std::vector<Player*>& objects);
	// Inherited via Item
	virtual void useItem() override;

};