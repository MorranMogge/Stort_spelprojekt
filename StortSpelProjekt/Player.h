#pragma once

#include "GameObject.h"
#include "Input.h"
#include "Potion.h"
#include "TimeStruct.h"
#include <GamePad.h>
#include <iostream>

#define FORCE 2500

class Item;

class Player: public GameObject
{
private:
	//Movement variables
	DirectX::SimpleMath::Vector3 resultVector;
	DirectX::XMMATRIX rotationMX;
	DirectX::XMFLOAT4X4 rotationFloat;
	bool controllerConnected = true;
	float jumpAllowed = 200.f;
	float jumpHeight = 200.f;

	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::SimpleMath::Vector3 normalVector = DEFAULT_UP;
	DirectX::SimpleMath::Vector3 rightVector = DEFAULT_RIGHT;
	DirectX::SimpleMath::Vector3 forwardVector = DEFAULT_FORWARD;

	const DirectX::XMVECTOR NORTH_EAST = DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR NORTH_WEST = DirectX::XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR SOUTH_EAST = DirectX::XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR SOUTH_WEST = DirectX::XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR northEastVector = NORTH_EAST;
	DirectX::XMVECTOR northWestVector = NORTH_WEST;
	DirectX::XMVECTOR southEastVector = SOUTH_EAST;
	DirectX::XMVECTOR southWestVector = SOUTH_WEST;

	void rotate();
	bool movingCross(const DirectX::XMVECTOR& cameraForward, float deltaTime);
	bool moveCrossController(const DirectX::XMVECTOR& cameraForward, float deltaTime);

	//Other variables
	const float speedConstant = 100.f;
	int repairCount = 0;
	Item* holdingItem;
	int health;
	float speed;
	bool dedge = false;
	TimeStruct timer;

	//Controller variables
	float posX = 0.0f;
	float posY = 0.0f;
	float totalPos = 0.0f;
	float throttle = 0.0f;

	void handleItems();
public:
	Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& grav);
	Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& grav);
	void handleInputs(); 
	void move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float deltaTime,  const bool& testingVec);
	void moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float deltaTime);
	bool getPickup(GameObject *pickup);
	bool pickupItem(Item *itemToPickup);

	void releasePickup();

	DirectX::XMVECTOR getUpVec() const;
	DirectX::XMVECTOR getForwardVec() const;
	DirectX::XMVECTOR getRightVec() const;
	DirectX::XMFLOAT4X4 getRotationMX();
	reactphysics3d::Vector3 getRayCastPos()const;

	void hitByBat(const reactphysics3d::Vector3& force);
	void addItem(Item* itemToHold);
	void addHealth(const int& healthToIncrease);
	void releaseItem();
	bool checkForStaticCollision(const std::vector<GameObject*>& gameObjects);
	bool raycast(const std::vector<GameObject*>& gameObjects, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal);
	bool withinRadius(Item* itemToLookWithinRadius, const float& radius) const;
	bool repairedShip() const;
	bool getHitByBat()const;
	void update();
};