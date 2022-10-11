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
	bool objectDraw = true;
	bool drawDebug = true;
	wirefameInfo reactWireframeInfo;
	ID3D11Buffer* wireBuffer;
	D3D11_MAPPED_SUBRESOURCE subData;

	std::unique_ptr<DirectX::GamePad> gamePad;

	float dt;
	std::chrono::time_point<std::chrono::system_clock> start;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	BasicRenderer basicRenderer;
	GravityField planetGravityField;

	PhysicsWorld physWolrd;
	SkyboxObj skybox;
	Camera camera;
	Player* player;
	GameObject* planet;
	GameObject* testCube;
	Potion* potion;			//not in use


	LightHandler ltHandler;
	PlayerVectors playerVecRenderer;

	//Objects
	std::vector<GameObject*> gameObjects;
	std::vector<ParticleEmitter> ptEmitters;
	
	

	void loadObjects();
	void drawShadows();
	void drawObjects(bool drawDebug);
	void drawParticles();
	bool setUpWireframe();
	void updateBuffers();
	void handleKeybinds();


public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

