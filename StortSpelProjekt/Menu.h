#pragma once
#define NOMINMAX
#include "State.h"
#include "MenuUI.h"
#include "BasicRenderer.h"
#include "Planet.h"
#include "SkyboxObj.h"
#include "LightHandler.h"
#include "ModelManager.h"

class Menu : public State
{
private:

	MenuUI ui;
	BasicRenderer basicRenderer;
	std::vector<Planet*> planets;
	std::vector<Mesh*> meshes;
	Camera cam;
	SkyboxObj skybox;
	LightHandler ltHandler;
	ModelManager manager;

	ID3D11Buffer* vBuff;
	ID3D11Buffer* iBuff;
	ID3D11Buffer* vBuff2;
	ID3D11Buffer* iBuff2;

	Mesh* tmpMesh;
	Mesh* tmpMesh2;

	std::vector<int> subMeshRanges;
	std::vector<int> subMeshRanges2;
	std::vector<int> verticies;
	std::vector<int> verticies2;

public:
	Menu();
	virtual ~Menu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	//void loadObjects();
	void drawShadows();
	void drawFresnel();
	void drawObjects();
	//void drawParticles();
	virtual void Render() override;
};

