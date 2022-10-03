#pragma once
#include "GameObject.h"
#include "Input.h"
#include "Potion.h"
#include <GamePad.h>
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
	Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	void handleInputs(); 
	void move(const DirectX::XMFLOAT3& grav, const DirectX::XMVECTOR& cameraRight, const std::unique_ptr<DirectX::GamePad>& gamePad, const float& deltaTime);
	bool getPickup(GameObject *pickup);
	bool pickupItem(Item *itemToPickup, const std::unique_ptr<DirectX::GamePad>& gamePad);
	void releasePickup();
	void addItem(Item* itemToHold);
	void addHealth(const int& healthToIncrease);
	void releaseItem();
	bool withinRadius(Item* itemToLookWithinRadius, const float& radius) const;
	void update(const std::unique_ptr<DirectX::GamePad>& gamePad);
};