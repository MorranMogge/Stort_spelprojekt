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
	HWND* window;

	Sound gameMusic;

	ImGuiHelper imGui;
	bool wireframe = false;
	bool objectDraw = true;
	bool drawDebug = false;
	bool landingMinigame = false;

	DirectX::GamePad* gamePad;
	GravityField* field;
	GravityField* oldField;
	bool changedPlanet = false;

	//Used for delta time
	float endTimer;
	float dt;
	std::chrono::time_point<std::chrono::system_clock> currentTime;
	std::chrono::time_point<std::chrono::system_clock> lastUpdate;
	
	//Server related variables
	std::chrono::time_point<std::chrono::system_clock> serverStart;
	float serverTimerLength =  1.f / 30.0f;
	Client* client;

	ModelManager manager;
	ID3D11Buffer* vBuff;
	ID3D11Buffer* iBuff;
	Mesh* tmpMesh;
	std::vector<int> subMeshRanges;
	std::vector<int> verticies;
	ID3D11ShaderResourceView* tempSRV;
	AnimationData animData;
	AnimatedMesh* sexyMan;

	//Gravity vector and velocity for the player (grav is "constant", velocity is "dynmic")
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 grav;

	BasicRenderer basicRenderer;
	GravityField* planetGravityField;
	PhysicsWorld physWorld;
	std::vector<Planet*> planetVector;
	AsteroidHandler* asteroids;

	PacketEventManager* packetEventManager;
	std::vector<Player*> players;
	std::vector<Item*> onlineItems;

	//variables to handle packets
	CircularBufferClient* circularBuffer;

	Camera camera;
	bool velocityCamera = false;

	Arrow* arrow;
	SkyboxObj skybox;
	Player* currentPlayer;
	Mesh* planetMeshes;

	//Items
	Potion* potion;
	BaseballBat* baseballBat;
	Grenade* grenade;

	//Objects
	std::vector<Component*> components;
	std::vector<GameObject*> gameObjects;
	std::vector<SpaceShip*> spaceShips;
	std::vector<Item*> items;
	std::vector< Mesh*> meshes;
	
	LightHandler ltHandler;
	ImGuiHelper* imguiHelper;
	PlayerVectors playerVecRenderer;
	std::vector<ParticleEmitter> ptEmitters;

	//HUD
	HudUI ui;


	void loadObjects();
	void drawShadows();
	void drawFresnel();
	void drawIcons();
	void drawObjects(bool drawDebug);
	void drawParticles();
	void updateBuffers();
	void handleKeybinds();
	void randomizeObjectPos(GameObject* item);

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

