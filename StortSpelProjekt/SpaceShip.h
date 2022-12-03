#pragma once
#include "GameObject.h"
#include "BilboardObject.h"
#include "Component.h"
#include "TimeStruct.h"
#include "CaptureZone.h"

class SpaceShip : public GameObject
{
private:
	//std::vector<GameObject*> components;
	BilboardObject* rocketStatusQuad;
	DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float moveCounter = 0.f;
	ParticleEmitter* particles;
	ParticleEmitter* particles2;
	int compToComplete;
	int currentComponents;
	TimeStruct timer;
	float counter;
	bool animate;
	int team;
	Sound compAddedSfx;
	Sound engineTakeOff;

	CaptureZone *zone;
	bool setUp = false;
public:

	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, Mesh* zoneMesh, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 4);
	SpaceShip(const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, Mesh* zoneMesh, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 4);
	~SpaceShip();
	int getTeam() const;
	int getNrOfComponents() const;//
	void addComponent();
	bool detectedComponent(GameObject* objectToCheck);
	bool detectedComponent(Component* componentToCheck);
	void animateOnPickup();
	void setAnimate(const bool& onOff);
	virtual void update() override;
	void drawQuad();
	bool getCompletion()const;
	void drawParticles();
	void drawFresnel();
	bool isFinished();
	void setSpaceShipRotationRelativePlanet(GravityField* field);
	virtual void draw() override;
	void flyAway(const float& deltaTime);
	void fly(const DirectX::XMFLOAT3& grav, const float& deltaTime);
};