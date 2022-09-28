#pragma once
#include "PhysicsWorld.h"
#include "State.h"
#include "BasicRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "GravityField.h"
#include <chrono>
#include "ShaderLoader.h"
#include "ImGuiHelper.h"

#include "Player.h"
#include "Camera.h"
#include "LightHandler.h"
#include "Light.h"
#include "StructuredBuffer.h"
#include "BufferTypes.h"
#include "LightHandler.h"
#include "Potion.h"
#include "Client.h"

struct wirefameInfo
{
	DirectX::XMFLOAT3 wireframeClr;
	float padding;
};

class Game : public State
{
private:
	ID3D11DeviceContext* immediateContext;

	bool wireframe = true;
	bool objectDraw = true;
	wirefameInfo reactWireframeInfo;
	ID3D11Buffer* wireBuffer;
	D3D11_MAPPED_SUBRESOURCE subData;

	float dt;
	std::chrono::time_point<std::chrono::system_clock> start;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	
	Client* client;

	BasicRenderer basicRenderer;
	GravityField planetGravityField;

	PhysicsWorld physWolrd;


	Player currentPlayer;
	std::vector<Player> players;

	Camera camera;
	GameObject* planet;
	Potion potion;


	LightHandler ltHandler;
	ImGuiHelper* imguiHelper;

	//Objects
	std::vector<GameObject> gameObjects;

	void loadObjects();
	void drawObjects();
	bool setUpWireframe();
	void updateBuffers();

	//float pos[3];

	//Variables for the mouse movement
	HWND* window;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window, ImGuiHelper& imguiHelper);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

