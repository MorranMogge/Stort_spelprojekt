#pragma once
#include "GameObject.h"
#include "Input.h"
#include "Potion.h"
#include <iostream>
using namespace DirectX;

class Player: public GameObject
{
private:
	const float speedConstant = 100.f;
	//Olivers order
	Item* holdingItem;
	//Camera
	DirectX::XMVECTOR playerUpVec;
	DirectX::XMVECTOR playerForwardVec;
	DirectX::XMVECTOR playerRightVec;
	int health;

public:
	Player(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Player(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	void handleInputs(); 
	void move(const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, float deltaTime);
	bool getPickup(GameObject *pickup);
	bool pickupItem(Item *itemToPickup);
	void releasePickup();
	void addItem(Item* itemToHold);
	void addHealth(const int& healthToIncrease);
	void releaseItem();
	bool withinRadius(Item* itemToLookWithinRadius, float radius) const;
	void update();
};