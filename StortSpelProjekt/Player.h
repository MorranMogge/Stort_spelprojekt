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
class SpaceShip;

class Player: public GameObject
{
private:
	//Movement variables
	float angle = 0.f;
	float speed = 25.f;
	float currentSpeed;
	const float speedConstant = 100.f;
	DirectX::SimpleMath::Vector3 resultVector;
	DirectX::SimpleMath::Vector3 angleVector;
	DirectX::SimpleMath::Vector3 velocity; //FINALLY ADDED THIS F*****G STUPID VARIABLE
	
	//Player rotation
	DirectX::XMMATRIX rotationMX;
	DirectX::XMFLOAT4X4 rotationFloat;

	//Status
	int team;
	int onlineID;
	bool dedge = false;
	bool flipping = false;
	bool onGround = false;
	bool holdingComp = false;
	bool moveKeyPressed = false;

	//Controller variables
	float posX = 0.0f;
	float posY = 0.0f;
	float throttle = 0.0f;
	float totalPos = 0.0f;
	bool controllerConnected = true;

	//Other variables
	Client* client;
	TimeStruct timer;
	Item* holdingItem;

	ParticleEmitter* particles;
	BilboardObject* playerIcon;

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

	void throwItem();
	void resetRotationMatrix();
	void handleItems();
	bool movingCross(const DirectX::XMVECTOR& cameraForward, float deltaTime);
	bool moveCrossController(const DirectX::XMVECTOR& cameraForward, float deltaTime);
public:
	Player(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, Client* client, const int &team, GravityField* field = nullptr);
	Player(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, Client* client, const int& team, GravityField* field = nullptr);
	~Player();

	//Move Functions
	void rotate(const DirectX::XMFLOAT3& grav, const bool& testingVec, const bool& changedPlanet);
	void move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const float& deltaTime);
	void moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const DirectX::XMFLOAT3& grav, const std::unique_ptr<DirectX::GamePad>& gamePad, float deltaTime);
	void updateVelocity(const DirectX::SimpleMath::Vector3& gravityVec);
	void resetVelocity();
	void velocityMove(const float& dt);
	
	//Set Functions
	void setSpeed(float speed);
	void setOnlineID(const int& id);
	void setTeam(const int& team);
	
	//Get Functions
	reactphysics3d::Vector3 getRayCastPos()const;
	DirectX::XMVECTOR getUpVector() const;
	DirectX::XMVECTOR getForwardVector() const;
	DirectX::XMVECTOR getRightVector() const;
	DirectX::XMMATRIX getRotationMX() const;
	int getTeam() const;
	int getOnlineID()const;
	bool getHitByBat()const;
	float getSpeed()const;

	//Item related functions
	bool pickupItem(Item *itemToPickup);
	Item* getItem()const;
	void addItem(Item* itemToHold);
	int getItemOnlineType()const;
	int getItemOnlineId()const;
	bool isHoldingItem()const;
	void releaseItem();
	bool isHoldingComp();

	//Collision and checks
	void hitByBat(const reactphysics3d::Vector3& force);
	bool checkForStaticCollision(const std::vector<Planet*>& gameObjects, const std::vector<SpaceShip*>& spaceShips);
	bool raycast(const std::vector<GameObject*>& gameObjects, const std::vector<Planet*>& planets, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal);
	bool withinRadius(Item* itemToLookWithinRadius, const float& radius) const;
	
	//Updating and rendering
	void drawIcon();
	void drawParticles();
	virtual void draw() override;
	void update();
	void requestingPickUpItem(const std::vector<Item*>& items);

	
	void itemRecvFromServer(Item* item);
};