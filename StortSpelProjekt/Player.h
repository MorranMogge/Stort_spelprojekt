#pragma once

#include "GameObject.h"
#include "Input.h"
#include "Potion.h"
#include <GamePad.h>
#include <iostream>

#define FORCE 2500

class Item;

class Player: public GameObject
{
private:
	//Movement variables
	DirectX::XMFLOAT3 dotValue;
	DirectX::XMVECTOR dotProduct;
	DirectX::XMMATRIX rotationMX;

	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;
	DirectX::XMVECTOR upVector = DEFAULT_UP;
	DirectX::XMVECTOR normalVector;
	bool keyboardMove = false;

	//Other shit
	const float speedConstant = 100.f;
	int repairCount = 0;
	Item* holdingItem;
	int health;
	float speed;

	void handleItems();
	void updatePhysCompRotation();

	//Controller shits
	float posX = 0.0f;
	float posY = 0.0f;
	float totalPos = 0.0f;
	float throttle = 0.0f;

public:
	Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	void handleInputs(); 
	void move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float deltaTime,  const bool& testingVec);
	void moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float& deltaTime);
	bool getPickup(GameObject *pickup);
	bool pickupItem(Item *itemToPickup);

	void releasePickup();

	DirectX::XMVECTOR getUpVec() const;
	DirectX::XMVECTOR getForwardVec() const;
	DirectX::XMVECTOR getRightVec() const;
	DirectX::XMMATRIX getRotationMX() const;
	reactphysics3d::Vector3 getRayCastPos()const;

	void addItem(Item* itemToHold);
	void addHealth(const int& healthToIncrease);
	void releaseItem();
	bool raycast(const std::vector<GameObject*>& gameObjects, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal);
	bool withinRadius(Item* itemToLookWithinRadius, const float& radius) const;
	bool repairedShip() const;
	void update();
};