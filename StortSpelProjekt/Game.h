#pragma once
#include "PhysicsWorld.h"
#include "State.h"
#include "Camera.h"
#include "BasicRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "GravityField.h"
#include <chrono>
#include "ShaderLoader.h"


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
	BasicRenderer basicRenderer;
	GravityField planetGravityField;

	PhysicsWorld physWolrd;

	//Objects
	std::vector<Mesh> meshes_Static;
	std::vector<Mesh> meshes_Dynamic;

	void loadObjects();
	void drawObjects();

	//Create Shape
	//Create Rigidbody
	//Create transform
	//Create collider, rigidbody->createCollider(&shape, transform)

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

