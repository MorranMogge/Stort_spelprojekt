#pragma once
#include "Item.h"
#include "TimeStruct.h"
#include "ConstantBufferNew.h"

class Grenade : public Item
{
private:
	std::vector<GameObject*> gameObjects;
	TimeStruct timer;
	bool destructionIsImminent;
	bool exploded;
	float timeToExplode;
	float currentTime;
	float counter;
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;
	Mesh* explosionMesh;
	Mesh* redMesh;
	Sound explosion;
	DirectX::XMFLOAT3 explodePosition;
	bool drawRed = false;
public:
	Grenade(Mesh* useMesh, Mesh* expMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	Grenade(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	~Grenade();
	void updateExplosionCheck();
	void setGameObjects(const std::vector<GameObject *> &gameObjects);
	virtual void drawIcon() override;
	virtual void drawParticles() override;
	virtual void drawFresnel() override;
	virtual void useItem() override;
	bool getExploded() const;
	void setExploded(const bool &onOff);
	virtual void draw() override;
private:
	void explode();

};
