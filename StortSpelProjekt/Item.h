#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "BilboardObject.h"
#include "TimeStruct.h"

//Lägga in mutex så att inte flera spelar kan ta samma item.
class Item : public GameObject
{
private:
	

protected:
	TimeStruct tStruct;
	BilboardObject* itemIcon;
	ParticleEmitter* particles;
	bool withinPlayerReach;
	bool pickedUp;

public:
	Item(Mesh* useMesh,const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3 & rot,const int & id, GravityField* field = nullptr);
	Item(const std::string & objectPath,const DirectX::XMFLOAT3 & pos,const DirectX::XMFLOAT3 & rot, const int & id, GravityField* field = nullptr);
	virtual ~Item();
	virtual void useItem() = 0;
	virtual void drawIcon();
	virtual void drawParticles();
	void setPickedUp(bool pickedUp);
	void checkDistance(GameObject* playerTocheck);
	//virtual void useItem();
	void throwItem();
	virtual void update() override;
};