#pragma once
#include "Item.h"
#include "TimeStruct.h"
#include "ConstantBufferNew.h"

class Player;

class Grenade : public Item
{
private:
	std::vector<GameObject*> gameObjects;
	TimeStruct timer;
	TimeStruct timer2;
	bool destructionIsImminent;
	bool exploded;
	float timeToExplode;
	float currentTime;
	float counter;
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;
	Mesh* explosionMesh;
	Sound explosion;
	DirectX::XMFLOAT3 explodePosition;

public:
	Grenade(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	~Grenade();
	void updateExplosionCheck();
	void setGameObjects(const std::vector<GameObject *> &gameObjects);
	virtual void drawIcon() override;
	virtual void drawParticles() override;
	void drawFresnel();
	virtual void useItem(const Player* playerHoldingItem) override;

	bool getExploded() const;
	void setExploded(const bool &onOff);
private:
	void explode();

};
