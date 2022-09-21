#pragma once
#include <reactphysics3d\reactphysics3d.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include "Vertex.h"
#include "ImGuiHelper.h"

class PhysicsWorld
{
private:
	UINT stride;
	UINT offset;

	std::vector<reactphysics3d::CollisionShape*> shapes; //Shapes tell us how the collider looks like
	std::vector<reactphysics3d::Collider*> colliders;	//Colliders lets us collide
	std::vector<reactphysics3d::RigidBody*> bodies;		//Rigidbody tells us where the collider is located

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

	std::vector<reactphysics3d::DebugRenderer::DebugTriangle> tri;
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
	void addForceToObject(const DirectX::XMFLOAT3& gravityVec);
	DirectX::SimpleMath::Vector3 getPos();
	DirectX::SimpleMath::Vector3 getRot();

	void addBoxToWorld(DirectX::XMFLOAT3 dimensions = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), float mass = 1.0f,  DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0));
	void addSphereToWorld(float radius = 0.5f, DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0));
	//void addSphereToWorld(reactphysics3d::decimal radius = 0.5f, DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, 0));
};