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
	BilboardObject* itemIcon;
	ParticleEmitter* particles;

public:
	Item(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, const int onlineId, int onlineType, GravityField* field = nullptr);
	Item(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, const int onlineId, int onlineType, GravityField* field = nullptr);
	virtual ~Item();
	virtual void useItem() = 0;
	void drawIcon();
	void setPickedUp(bool pickedUp);
	void drawParticles();
	//virtual void useItem();
	void throwItem();
	int getOnlineType()const;
	int getOnlineId()const;
	virtual void update() override;
};