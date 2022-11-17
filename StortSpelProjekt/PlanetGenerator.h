#pragma once
#include "State.h"
#include "Planet.h"
#include "BasicRenderer.h"
#include "ImGuiHelper.h"

class PlanetGenerator : public State
{
private:
	Planet* planet;
	std::vector<Mesh*> meshes;

	BasicRenderer renderer;
	ImGuiHelper imGuiHelper;

	std::vector<Vertex *> vertices;

public:
	PlanetGenerator();
	virtual  ~PlanetGenerator() override;

	virtual GAMESTATE Update() override;
	virtual void Render() override;
};
