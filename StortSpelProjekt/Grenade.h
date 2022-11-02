#pragma once
#include "Item.h"
#include "TimeStruct.h"
#include "ConstantBufferNew.h"

class Grenade : public Item
{
private:
	std::vector<GameObject*> gameObjects;
	TimeStruct timer;
	TimeStruct timer2;
	bool destructionIsImminent;
	float timeToExplode;
	float currentTime;
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;
	//ConstantBufferNew<posStruct> upVectorBuffer;

	void explode();
public:
	Grenade(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field = nullptr);
	Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field = nullptr);
	~Grenade();
	void updateExplosionCheck();
	void setGameObjects(const std::vector<GameObject *> &gameObjects);
	virtual void drawIcon() override;
	virtual void drawParticles() override;
	// Inherited via Item
	virtual void useItem() override;
};
