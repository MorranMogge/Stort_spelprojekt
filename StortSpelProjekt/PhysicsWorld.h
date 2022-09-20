#pragma once
#include <reactphysics3d\reactphysics3d.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>
#include "Vertex.h"

class PhysicsWorld
{
private:
	UINT stride;
	UINT offset;

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
};