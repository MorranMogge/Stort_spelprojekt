#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "BilboardObject.h"
#include "TimeStruct.h"

//Lägga in mutex så att inte flera spelar kan ta samma item.
class Item : public GameObject
{
private:
	bool pickedUp;
	TimeStruct tStruct;

protected:
	BilboardObject* itemIcon;
	ParticleEmitter* particles;
	bool withinPlayerReach;
public:
	Item(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field = nullptr);
	Item(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field = nullptr);
	virtual ~Item();
	virtual void useItem() = 0;
	void drawIcon();
	void setPickedUp(bool pickedUp);
	void drawParticles();
	void checkDistance(GameObject* playerTocheck);
	//virtual void useItem();
	void throwItem();
	virtual void update() override;
};