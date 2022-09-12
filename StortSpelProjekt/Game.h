#pragma once
#include "State.h"
#include "Camera.h"
#include "BasicRenderer.h"
#include "Mesh.h"

class Game : public State
{
private:
	ID3D11DeviceContext* immediateContext;

	Camera camera;
	BasicRenderer basicRenderer;

	//Objects
	std::vector<Mesh> meshes_Static;
	std::vector<Mesh> meshes_Dynamic;

	void loadObjects();
	void drawObjects();

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

