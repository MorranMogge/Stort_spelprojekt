#pragma once
#include "GameObject.h"
#include "Input.h"
#include <iostream>
using namespace DirectX;

class Player: public GameObject
{
private:

	GameObject* pickup;

	//Movement
	DirectX::XMVECTOR playerUpVec;
	DirectX::XMVECTOR playerForwardVec;
	DirectX::XMVECTOR playerRightVec;

	DirectX::XMVECTOR forwardVec;
	DirectX::XMVECTOR zVec = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR dotVector;
	DirectX::XMVECTOR tempRightVec = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

public:
	Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	void handleInputs(); 
	void move(DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& rotation, const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, float deltaTime);
	bool getPickup(GameObject *pickup);
	void releasePickup();
};