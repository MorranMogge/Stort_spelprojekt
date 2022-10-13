#pragma once
#include "GameObject.h"

class SpaceShip : public GameObject
{
private:
	std::vector<GameObject*> components;
	int nrOfComponents;
public:
	SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1),const int & nrofComp = 3);
	SpaceShip(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1), const int & nrofComp = 3);
	~SpaceShip();
	bool detectedComponent(GameObject* objectToCheck);
};
