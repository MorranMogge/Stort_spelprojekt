#pragma once
#include "GameObject.h"
#include "Input.h"
#include <iostream>

class Player: public GameObject
{
private:

	GameObject* pickup;

public:
	Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	void handleInputs();
	void move(float deltaTime);
};