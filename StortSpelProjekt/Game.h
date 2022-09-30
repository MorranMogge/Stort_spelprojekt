#pragma once
#include "GameInclude.h"

struct wirefameInfo
{
	DirectX::XMFLOAT3 wireframeClr;
	float padding;
};

class Game : public State
{
private:
	ID3D11DeviceContext* immediateContext;

	ImGuiHelper imGui;
	bool wireframe = true;
	bool objectDraw = false;
	wirefameInfo reactWireframeInfo;
	ID3D11Buffer* wireBuffer;
	D3D11_MAPPED_SUBRESOURCE subData;

	float dt;
	std::chrono::time_point<std::chrono::system_clock> start;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	BasicRenderer basicRenderer;
	GravityField planetGravityField;

	PhysicsWorld physWolrd;

	Camera camera;
	Player player;
	GameObject* planet;
	GameObject* testCube;
	std::vector<GameObject*> testObjects;
	Potion potion;


	LightHandler ltHandler;

	//Objects
	std::vector<GameObject> gameObjects;

	void loadObjects();
	void drawObjects();
	bool setUpWireframe();
	void updateBuffers();


	//Variables for the mouse movement
	MouseClass* mouse;
	HWND* window;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

