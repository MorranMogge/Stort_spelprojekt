#pragma once
#include "State.h"
#include "LoseUI.h"
#include "BasicRenderer.h"
#include "SkyboxObj.h"
#include "LightHandler.h"
#include "ModelManager.h"

class LoseMenu : public State
{
	LoseUI ui;
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

	AnimatedMesh* sexyMan;
	AnimatedMesh* sexyMan2;

	void drawFresnel();
	void drawObjects();

public:

	LoseMenu(int& currentTeam);
	virtual ~LoseMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
