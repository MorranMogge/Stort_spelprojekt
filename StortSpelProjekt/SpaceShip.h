#pragma once
#include "GameObject.h"
#include "BilboardObject.h"
#include "Component.h"
#include "TimeStruct.h"

class SpaceShip : public GameObject
{
private:
	//std::vector<GameObject*> components;
	BilboardObject* rocketStatusQuad;
	ParticleEmitter* particles;
	ParticleEmitter* particles2;
	int compToComplete;
	int currentComponents;
	TimeStruct timer;
	float counter;
	bool animate;
	int team;

public:

	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 4);
	SpaceShip(const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 4);
	~SpaceShip();
	int getNrOfComponents();
	bool detectedComponent(GameObject* objectToCheck);
	bool detectedComponent(Component* componentToCheck);
	void addComponent();
	void takeOff();
	void animateOnPickup();
	void setAnimate(const bool& onOff);
	virtual void update() override;
	void drawQuad();
	void drawParticles();
	bool isFinished();
	virtual void draw() override;
};
