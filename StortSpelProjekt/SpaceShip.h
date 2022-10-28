#pragma once
#include "GameObject.h"
#include "BilboardObject.h"
#include "Component.h"

class SpaceShip : public GameObject
{
private:
	std::vector<GameObject*> components;
	BilboardObject* rocketStatusQuad;
	int compToComplete;
	int currentComponents;
public:

	inline static SpaceShip* red = nullptr;
	inline static SpaceShip* blue = nullptr;
	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 4);
	SpaceShip(const DirectX::XMFLOAT3& pos, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 4);
	~SpaceShip();
	int getNrOfComponents();
	bool detectedComponent(GameObject* objectToCheck);
	bool detectedComponent(Component* componentToCheck);
	void addComponent();
	virtual void update() override;
	void drawQuad();
	bool isFinished();
};
