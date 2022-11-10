#pragma once
#include "PhysicsComponent.h"
#include "Vertex.h"	//Used for creating the rendering triangles
#include "Planet.h"

class GameObject;

class PhysicsWorld
{
private:
	int testingWarnings;
	UINT stride;
	UINT offset;
	reactphysics3d::uint64 nrOfTriangles;
	std::vector<Planet *> planets;   

	//Used for gravity
	DirectX::XMFLOAT3 grav;
	reactphysics3d::Vector3 temp;

	std::vector<PhysicsComponent *> physObjects;
		
	reactphysics3d::PhysicsCommon com;
	reactphysics3d::PhysicsWorld* world;

	reactphysics3d::RigidBody* rigid;
	reactphysics3d::DebugRenderer* debugRenderer;

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
	void addForceToObjects(const float& dt);

public:
	PhysicsWorld(std::string worldName = "Planet SIS");
	~PhysicsWorld();
	void setPlanets(std::vector<Planet *>& planets);
	void update(const float& dt);
	void renderReact3D();
	PhysicsComponent* getPlayerBox()const;
	void addBoxToWorld(DirectX::XMFLOAT3 dimensions = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), float mass = 1.0f,  DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 10, 00));
	void addPhysComponent(GameObject* gameObj, reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
	PhysicsComponent* returnAddedPhysComponent(reactphysics3d::CollisionShapeName shape = reactphysics3d::CollisionShapeName::BOX, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f), const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));
};