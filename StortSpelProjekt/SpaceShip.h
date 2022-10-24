#pragma once
#include "GameObject.h"
#include "BilboardObject.h"

class SpaceShip : public GameObject
{
private:
	std::vector<GameObject*> components;
	int nrOfComponents;
	BilboardObject* rocketStatusQuad;
	int compToComplete = 4;

public:

	inline static SpaceShip* red = nullptr;
	inline static SpaceShip* blue = nullptr;

	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const DirectX::XMFLOAT3& direction, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 3);
	SpaceShip(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const DirectX::XMFLOAT3& direction, const int& id, const int team, GravityField* field, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 3);
	~SpaceShip();

	int getNrOfComponents() { return (int)components.size(); }
	bool detectedComponent(GameObject* objectToCheck);
	virtual void update() override;
	void drawQuad();
	bool isFinished();
};
