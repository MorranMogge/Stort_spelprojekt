#pragma once
#include "GameObject.h"
#include "Input.h"
#include <iostream>

class Player: public GameObject
{
private:
	GameObject* pickup;

	//Movement variables
	DirectX::XMFLOAT3 dotValue;
	DirectX::XMVECTOR dotProduct;

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR normalVector;
	DirectX::XMVECTOR upVector = DEFAULT_UP;
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;

public:
	Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	void handleInputs(); 
	void move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, DirectX::SimpleMath::Vector3& position, DirectX::XMMATRIX& rotation, DirectX::XMMATRIX& rotationMX, const DirectX::XMFLOAT3& grav, float& deltaTime);
	bool getPickup(GameObject *pickup);
	void releasePickup();

	DirectX::XMVECTOR getUpVec() const;
	DirectX::XMVECTOR getForwardVec() const;
	DirectX::XMVECTOR getRightVec() const;
};