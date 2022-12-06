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

	void drawFresnel();
	void drawObjects();
public:

	WinMenu();
	virtual ~WinMenu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
