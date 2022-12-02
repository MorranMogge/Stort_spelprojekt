#pragma once

#include "GameObject.h"
#include "Input.h"
#include "BilboardObject.h"
#include "ParticleEmitter.h"
#include "Potion.h"
#include "TimeStruct.h"
#include "Client.h"
#include "AnimatedMesh.h"
#include "Planet.h"

#include <GamePad.h>
#include <iostream>
#define FORCE 2500

class Item;
class SpaceShip;
class Component;

class Player: public AnimatedMesh
{
private:
	//Movement variables
	float angle = 0.f;
	float speed = 20.f;
	float currentSpeed = 0.f;
	DirectX::SimpleMath::Vector3 resultVector;
	DirectX::SimpleMath::Vector3 angleVector;
	DirectX::SimpleMath::Vector3 velocity; //FINALLY ADDED THIS F*****G STUPID VARIABLE
	
	//Player rotation
	DirectX::XMMATRIX rotationMX;
	DirectX::XMFLOAT4X4 rotationFloat;

	//SFX
	Sound pickUpSfx;
	Sound walkingSound;
	Sound playerHitSound;

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
	DirectX::GamePad* gamePad;
	DirectX::GamePad::ButtonStateTracker tracker;
	DirectX::GamePad::State state;

	//Other variables
	Client* client;
	TimeStruct timer;
	TimeStruct keyPressTimer;
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

	DirectX::SimpleMath::Vector3 playerVector;
	DirectX::SimpleMath::Vector3 planetVector;

	int animIndex = 0;
	bool eKeyDown = false;
	float animSpeed = 1;
	bool usingBat = false;
	TimeStruct dropTimer;
	bool usedItem = true;
	bool throwingItem = false;

	void throwItem();
	void resetRotationMatrix();
	void handleItems();
	bool movingCross(const DirectX::XMVECTOR& cameraForward, float deltaTime);
	bool moveCrossController(const DirectX::XMVECTOR& cameraForward, float deltaTime);
public:
	Player(Mesh* useMesh, const AnimationData& data, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, Client* client, const int& team,
		ID3D11ShaderResourceView* redTeamColor, ID3D11ShaderResourceView* blueTeamColor, GravityField* field = nullptr);
	~Player();

	//Move Functions
	void rotate(const DirectX::XMFLOAT3& grav, const bool& testingVec, const bool& changedPlanet);
	void move(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const float& deltaTime);
	void moveController(const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraRight, const float& deltaTime);
	void updateVelocity(const DirectX::SimpleMath::Vector3& gravityVec);
	void resetVelocity();
	void velocityMove(const float& dt);
	
	//Set Functions
	void setSpeed(float speed);
	void setOnlineID(const int& id);
	void setTeam(const int& team);
	void setVibration(float vibration1, float vibration2);
	void setGamePad(DirectX::GamePad* gamePad);
	
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
	void getAnimSpeed(float& speed);

	//Item related functions
	bool pickupItem(const std::vector <Item *>& items, const std::vector <Component*>& components);
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
	void checkSwimStatus(const std::vector<Planet*>& planets);
	bool raycast(const std::vector<GameObject*>& gameObjects, const std::vector<Planet*>& planets, DirectX::XMFLOAT3& hitPos, DirectX::XMFLOAT3& hitNormal);
	bool withinRadius(Item* itemToLookWithinRadius, const float& radius) const;
	void colliedWIthComponent(const std::vector<Component*>& components);

	void stateMachine(const float dt);
	void giveItemMatrix();
	
	//Updating and rendering
	void drawIcon();
	void drawParticles();
	//virtual void draw() override;
	void update();
	void requestingPickUpItem(const std::vector<Item*>& items);
	void updateController();
	
	void itemRecvFromServer(Item* item);
};