#pragma once
#include <reactphysics3d\reactphysics3d.h>
#include "State.h"
#include "BasicRenderer.h"
#include "Mesh.h"
#include "Input.h"
//#include "MouseClass.h"
#include "GravityField.h"
#include <chrono>
#include "Player.h"
#include "Camera.h"
#include "LightHandler.h"
#include "Light.h"
#include "StructuredBuffer.h"
#include "BufferTypes.h"
#include "LightHandler.h"
#include "Client.h"

class Game : public State
{
private:
	ID3D11DeviceContext* immediateContext;

	float dt;
	std::chrono::time_point<std::chrono::system_clock> start;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	
	Client* client;

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

	Camera camera;
	Player player;
	LightHandler ltHandler;

	//Objects
	std::vector<Mesh> meshes_Static;
	std::vector<Mesh> meshes_Dynamic;
	std::vector<GameObject> gameObjects;

	void loadObjects();
	void drawObjects();
	void setUpReact3D();

	//Create Shape
	//Create Rigidbody
	//Create transform
	//Create collider, rigidbody->createCollider(&shape, transform)

	float pos[3];

	//Variables for the mouse movement
	//MouseClass* mouse;
	HWND* window;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window, Client*& client);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

