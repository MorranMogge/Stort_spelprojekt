#pragma once
#define NOMINMAX
#include "PhysicsWorld.h"
#include "State.h"
#include "BasicRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "MouseClass.h"
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
#include "AnimatedMesh.h"
#include "GameInclude.h"
#include "ModelManager.h"

struct wirefameInfo
{
	DirectX::XMFLOAT3 wireframeClr;
	float padding;
};

class Game : public State
{
private:

	ID3D11Buffer* vBuff;
	ID3D11Buffer* iBuff;
	Mesh* tmpMesh;
	std::vector<int> subMeshRanges;
	std::vector<int> verticies;
	ID3D11ShaderResourceView* tempSRV;
	AnimationData animData;


	ID3D11DeviceContext* immediateContext;
	ModelManager manager;
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
	Player* tmp;
	Potion* tmp2;
	GameObject* planet;
	AnimatedMesh* sexyMan;
	GameObject* testCube;
	SpaceShip* spaceShip;
	Potion* potion;			//not in use
	BaseballBat* testBat;
	Player* otherPlayer;

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
	DirectX::SimpleMath::Vector3 orientToPlanet(const DirectX::XMFLOAT3 &position);


public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};