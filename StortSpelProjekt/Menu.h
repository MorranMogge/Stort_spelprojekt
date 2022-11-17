#pragma once
#include "State.h"
#include "MenuUI.h"
#include "BasicRenderer.h"
#include "Planet.h"
#include "SkyboxObj.h"
#include "LightHandler.h"

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

public:
	Menu();
	virtual ~Menu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

