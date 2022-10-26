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

	std::chrono::time_point<std::chrono::system_clock> serverStart;
	
	float serverTimerLength =  1.f / 30.0f;
	Client* client;

	BasicRenderer basicRenderer;
	GravityField planetGravityField;

	PhysicsWorld physWolrd;


	PacketEventManager* packetEventManager;
	std::vector<Player*> players;

	//variables to handle packets
	CircularBufferClient* circularBuffer;


	Camera camera;
	SkyboxObj skybox;
	Player* currentPlayer;
	GameObject* planet;
	GameObject* testCube;
	SpaceShip* spaceShip;
	Potion* potion;			//not in use
	BaseballBat* testBat;
	Player* otherPlayer;

	LightHandler ltHandler;
	ImGuiHelper* imguiHelper;
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
	void randomizeObjectPos(GameObject* item);
	DirectX::SimpleMath::Vector3 orientToPlanet(const DirectX::XMFLOAT3 &position);
	HWND* window;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

