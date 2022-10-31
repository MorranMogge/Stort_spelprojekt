#pragma once
#include "GameObject.h"
#include "BilboardObject.h"
#include "Component.h"

class SpaceShip : public GameObject
{
private:
	std::vector<GameObject*> components;
	BilboardObject* rocketStatusQuad;
	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	ParticleEmitter* particles;
	int compToComplete;
	int currentComponents;
public:

	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 4);
	SpaceShip(const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 4);
	~SpaceShip();
	int getNrOfComponents();
	void addComponent();
	bool detectedComponent(GameObject* objectToCheck);
	bool detectedComponent(Component* componentToCheck);
	void takeOff();
	virtual void update() override;
	void drawQuad();
	bool getCompletion()const;
	void drawParticles();
	bool isFinished();
	void move(const DirectX::XMFLOAT3& grav, const float& deltaTime);
};