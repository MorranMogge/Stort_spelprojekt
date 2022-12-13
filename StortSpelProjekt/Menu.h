#pragma once
#define NOMINMAX
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
	std::chrono::time_point<std::chrono::system_clock> timer;
	float time;
	ID3D11ShaderResourceView* loadTexture(const std::string& fileName);
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

