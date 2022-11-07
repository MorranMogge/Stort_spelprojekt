#pragma once
#include "GameInclude.h"


struct wirefameInfo
{
	DirectX::XMFLOAT3 wireframeClr;
	float padding;
};

const int NROFPLAYERS = 1;
static bool IFONLINE = false;

class Game : public State
{
private:
	ID3D11DeviceContext* immediateContext;

	ImGuiHelper imGui;
	bool wireframe = false;
	bool objectDraw = true;
	bool drawDebug = false;
	wirefameInfo reactWireframeInfo;
	ID3D11Buffer* wireBuffer;
	D3D11_MAPPED_SUBRESOURCE subData;

	std::unique_ptr<DirectX::GamePad> gamePad;
	float endTimer;
	float dt;
	std::chrono::time_point<std::chrono::system_clock> currentTime;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	std::chrono::time_point<std::chrono::system_clock> serverStart;
	
	std::vector< Mesh*> meshes;

	float serverTimerLength =  1.f / 30.0f;
	Client* client;

	BasicRenderer basicRenderer;
	GravityField planetGravityField;
	PhysicsWorld physWolrd;


	PacketEventManager* packetEventManager;
	std::vector<Player*> players;
	std::vector<Item*> onlineItems;

	//variables to handle packets
	CircularBufferClient* circularBuffer;

	Camera camera;
	SkyboxObj skybox;
	Player* currentPlayer;
	GameObject* atmosphere;
	std::vector<Component*> components;
	std::vector<SpaceShip*> spaceShips;

	LightHandler ltHandler;
	ImGuiHelper* imguiHelper;
	PlayerVectors playerVecRenderer;

	//Objects
	std::vector<GameObject*> gameObjects;
	std::vector<Item*> items;
	std::vector<ParticleEmitter> ptEmitters;
	
	//HUD
	HudUI ui;
	
	//Temp buffer for atmosphere
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;


	void loadObjects();
	void drawShadows();
	void drawFresnel();
	void drawIcons();
	void drawObjects(bool drawDebug);
	void drawParticles();
	bool setUpWireframe();
	void updateBuffers();
	void handleKeybinds();
	void randomizeObjectPos(GameObject* item);
	HWND* window;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

