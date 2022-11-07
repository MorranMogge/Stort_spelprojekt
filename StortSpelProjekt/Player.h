#pragma once

#include "GameObject.h"
#include "Input.h"
#include "BilboardObject.h"
#include "ParticleEmitter.h"
#include "Potion.h"
#include "TimeStruct.h"
#include "Client.h"
#include "Planet.h"

#include <GamePad.h>
#include <iostream>
#define FORCE 2500

class Item;

class Player: public GameObject
{
private:
	//Movement variables
	const float speedConstant = 100.f;
	float speed = 25.f;
	DirectX::SimpleMath::Vector3 resultVector;
	DirectX::SimpleMath::Vector3 velocity; //FINALLY ADDED THIS F*****G STUPID VARIABLE
	
	//Player rotation
	DirectX::XMMATRIX rotationMX;
	DirectX::XMFLOAT4X4 rotationFloat;

	//Status
	bool onGround = false;
	bool dedge = false;
	bool moveKeyPressed = false;
	bool holdingComp = false;
	int onlineID;
	int team;
	float jumpAllowed = 300.f;
	float jumpHeight = 300.f;


	//Controller variables
	bool controllerConnected = true;
	float posX = 0.0f;
	float posY = 0.0f;
	float totalPos = 0.0f;
	float throttle = 0.0f;

	//Other variables
	Client* client;
	Item* holdingItem;
	TimeStruct timer;

	BilboardObject* playerIcon;
	ParticleEmitter* particles;

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

	void resetRotationMatrix();
	void handleItems();
	void rotate();
	bool movingCross(const DirectX::XMVECTOR& cameraForward, float deltaTime);
	bool moveCrossController(const DirectX::XMVECTOR& cameraForward, float deltaTime);
public:
	Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, Client* client, const int &team, GravityField* field = nullptr);
	Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, Client* client, const int& team, GravityField* field = nullptr);
	~Player();

	//Move Functions

	void move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, float deltaTime,  const bool& testingVec);
	void moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float deltaTime);
	void checkMovement();
	void updateVelocity(const DirectX::SimpleMath::Vector3& gravityVec);
	void resetVelocity();
	
	//Set Functions

	void setSpeed(float speed);
	void setOnlineID(const int& id);
	void setTeam(const int& team);
	

	//Get Functions

	reactphysics3d::Vector3 getRayCastPos()const;
	DirectX::XMVECTOR getUpVec() const;
	DirectX::XMVECTOR getForwardVec() const;
	DirectX::XMVECTOR getRightVec() const;
	DirectX::XMMATRIX getRotationMX();
	int getTeam() const;
	int getOnlineID()const;
	bool getHitByBat()const;

	//Item related functions

	bool pickupItem(Item *itemToPickup);
	Item* getItem()const;
	void addItem(Item* itemToHold);
	int getItemOnlineType()const;
	int getItemOnlineId()const;
	void releaseItem();

	//Collision and checks

	void hitByBat(const reactphysics3d::Vector3& force);
	bool checkForStaticCollision(const std::vector<GameObject*>& gameObjects);
	bool raycast(const std::vector<GameObject*>& gameObjects, const std::vector<Planet*>& planets, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal);
	bool withinRadius(Item* itemToLookWithinRadius, const float& radius) const;
	
	void drawIcon();
	void drawParticles();

	void update();
};