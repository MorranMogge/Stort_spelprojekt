#pragma once
#include "GameObject.h"
#include "Input.h"
#include <iostream>
using namespace DirectX;

class Player: public GameObject
{
private:
	GameObject* pickup;

	//Movement variables
	XMFLOAT3 dotValue;
	DirectX::XMVECTOR dotProduct;
	DirectX::XMMATRIX rotationMX;

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR upVector = DEFAULT_UP;
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;

	//Calculations only done once
	float northWest = XM_PI + XM_PIDIV2 + XM_PIDIV4;
	float northEast = XM_PIDIV4;
	float southWest = XM_PI + XM_PIDIV4;
	float southEast = XM_PIDIV2 + XM_PIDIV4;
	float piDiv2 = XM_PI + XM_PIDIV2;

public:
	Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	void handleInputs(); 
	void move(DirectX::XMVECTOR cameraUp, DirectX::SimpleMath::Vector3& position, DirectX::SimpleMath::Vector3& rotation, const DirectX::XMFLOAT3& grav, float deltaTime);
	bool getPickup(GameObject *pickup);
	void releasePickup();

	DirectX::XMVECTOR getUpVector();
	DirectX::XMVECTOR getForwardVector();
	DirectX::XMVECTOR getRightVector();
};