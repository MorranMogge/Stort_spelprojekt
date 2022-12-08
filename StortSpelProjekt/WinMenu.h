#pragma once
#include "State.h"
#include "WinUI.h"
#include "BasicRenderer.h"
#include "SkyboxObj.h"
#include "LightHandler.h"
#include "ModelManager.h"

class WinMenu : public State
{
	WinUI ui;
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

	ID3D11ShaderResourceView* loadTexture(const std::string& fileName);
	void drawFresnel();
	void drawObjects();
public:

	WinMenu();
	virtual ~WinMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
