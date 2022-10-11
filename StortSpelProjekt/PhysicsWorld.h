#pragma once
#include "PhysicsComponent.h"

#include "Vertex.h"	//Used for creating the rendering triangles

class GameObject;

class PhysicsWorld
{
private:
	UINT stride;
	UINT offset;
	reactphysics3d::uint64 nrOfTriangles;

	//Used for gravity
	DirectX::XMFLOAT3 grav;
	reactphysics3d::Vector3 temp;

	std::vector<PhysicsComponent *> physObjects;

	reactphysics3d::PhysicsCommon com;
	reactphysics3d::PhysicsWorld* world;

	reactphysics3d::RigidBody* rigid;
	reactphysics3d::DebugRenderer* debugRenderer;
	reactphysics3d::SphereShape* planetShape;
	reactphysics3d::Collider* planetCollider;
	reactphysics3d::BoxShape* playerShape;
	reactphysics3d::Collider* playerCollider;

	reactphysics3d::RigidBody* playerRigidBody;
	reactphysics3d::RigidBody* planetRigidBody;

	PhysicsComponent* playerBox;

	std::vector<Vertex> triangles;

	ID3D11Buffer* debuggerBuffer;
	ID3D11PixelShader* dpShader;
	ID3D11RasterizerState* wireframeMode;

	DirectX::XMFLOAT4X4 identityM;
	ID3D11Buffer* identityMatrix;

	bool recreateVertexBuffer();
	bool setUpWireframe();
	bool setVertexBuffer();
	void setUpBaseScenario();
	void updateVertexBuffer();

public:
	PhysicsWorld(std::string worldName = "Planet SIS");
	~PhysicsWorld();
	void update(float dt);
	void renderReact3D();
	void addForceToObjects();
	DirectX::SimpleMath::Vector3 getPos() const;
	DirectX::SimpleMath::Vector3 getRot() const;
	PhysicsComponent* getPlayerBox()const;
	void updatePlayerBox(const DirectX::SimpleMath::Vector3& pos);
	void addBoxToWorld(DirectX::XMFLOAT3 dimensions = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), float mass = 1.0f,  DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 10, 00));
	void addSphereToWorld(float radius = 0.5f, DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0));
	void addPhysComponent(GameObject* gameObj, reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX);
};