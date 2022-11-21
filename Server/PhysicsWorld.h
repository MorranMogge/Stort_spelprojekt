#pragma once
#include "PhysicsComponent.h"
#include <string>
#include <vector>
#include "Item.h"

class GameObject;
class Planet;

class PhysicsWorld
{
private:

	//Used for gravity
	DirectX::XMFLOAT3 grav;
	reactphysics3d::Vector3 temp;

	std::vector<PhysicsComponent *> physObjects;
	std::vector<Planet*> planets;
	reactphysics3d::PhysicsCommon com;
	reactphysics3d::PhysicsWorld* world;

	reactphysics3d::RigidBody* rigid;

	PhysicsComponent* playerBox;

	void setUpBaseScenario();
	void addForceToObjects(const float& dt);

public:
	PhysicsWorld(std::string worldName = "Planet SIS");
	~PhysicsWorld();
	void setPlanets(std::vector<Planet*>& planets);
	void update(const float& dt);
	void renderReact3D();
	PhysicsComponent* getPlayerBox()const;
	void addBoxToWorld(DirectX::XMFLOAT3 dimensions = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), float mass = 1.0f,  DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 10, 00));
	void addPhysComponent(Item& pos, reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	void addPhysComponent(PhysicsComponent* newComp, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0, 50, 0), reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	PhysicsComponent* returnAddedPhysComponent(reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f), const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
};