#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"
#include "BilboardObject.h"

//Lägga in mutex så att inte flera spelar kan ta samma item.
class Item : public GameObject
{
private:
	bool pickedUp;

protected:
	BilboardObject* itemIcon;
	ParticleEmitter* particles;

public:
	Item(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field = nullptr);
	Item(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, GravityField* field = nullptr);
	virtual ~Item();
	virtual void useItem() = 0;
	void drawIcon();
	void setPickedUp(bool pickedUp);
	void drawParticles();
	//virtual void useItem();
	void throwItem();
	virtual void update() override;
};