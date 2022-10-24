#pragma once
#include "GameObject.h"
#include "BilboardObject.h"

class SpaceShip : public GameObject
{
private:
	std::vector<GameObject*> components;
	BilboardObject* rocketStatusQuad;
	int compToComplete = 4;

public:
	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 4);
	SpaceShip(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 4);
	~SpaceShip();
	bool detectedComponent(GameObject* objectToCheck);
	virtual void update() override;
	void drawQuad();
	bool isFinished();
};
