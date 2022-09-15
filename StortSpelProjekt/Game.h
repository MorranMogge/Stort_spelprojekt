#pragma once
#include <reactphysics3d\reactphysics3d.h>
#include "State.h"
#include "Camera.h"
#include "BasicRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "MouseClass.h"
#include "GravityField.h"
#include <chrono>


class Game : public State
{
private:
	ID3D11DeviceContext* immediateContext;

	float dt;
	std::chrono::time_point<std::chrono::system_clock> start;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	Camera camera;
	DirectX::XMVECTOR playerUpVec;
	BasicRenderer basicRenderer;
	GravityField planetGravityField;

	reactphysics3d::PhysicsCommon com;
	reactphysics3d::PhysicsWorld* world;

	reactphysics3d::RigidBody* rigid;

	reactphysics3d::SphereShape* planetShape;
	reactphysics3d::Collider* planetCollider;
	reactphysics3d::BoxShape* playerShape;
	reactphysics3d::Collider* playerCollider;

	reactphysics3d::RigidBody* playerRigidBody;
	reactphysics3d::RigidBody* planetRigidBody;


	//Objects
	std::vector<Mesh> meshes_Static;
	std::vector<Mesh> meshes_Dynamic;

	void loadObjects();
	void drawObjects();
	void setUpReact3D();

	//Create Shape
	//Create Rigidbody
	//Create transform
	//Create collider, rigidbody->createCollider(&shape, transform)

	float pos[3];

	//Variables for the mouse movement
	MouseClass* mouse;
	POINT mousePos = {};
	HWND* window;
	RECT rect;
	POINT ul;
	POINT lr;
	MSG* msg;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

