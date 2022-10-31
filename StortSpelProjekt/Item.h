#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "BilboardObject.h"
#include "TimeStruct.h"

//L�gga in mutex s� att inte flera spelar kan ta samma item.
class Item : public GameObject
{
private:
	bool pickedUp;
	TimeStruct tStruct;
	int onlineType;
	int onlineId;

protected:
	TimeStruct tStruct;
	BilboardObject* itemIcon;
	ParticleEmitter* particles;
	bool withinPlayerReach;
	bool pickedUp;

public:
	Item(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, const int& onlineType, GravityField* field = nullptr);
	Item(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, const int& onlineType, GravityField* field = nullptr);
	virtual ~Item();
	virtual void useItem() = 0;
	virtual void drawIcon();
	virtual void drawParticles();
	void setPickedUp(bool pickedUp);
	void checkDistance(GameObject* playerTocheck);
	//virtual void useItem();
	void throwItem();
	int getOnlineType()const;
	int getOnlineId()const;
	virtual void update() override;
};