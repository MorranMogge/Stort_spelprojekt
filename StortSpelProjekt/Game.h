#pragma once
#include "State.h"
#include "Camera.h"
#include "BasicRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "MouseClass.h"

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

	float pos[3];

	//Variables for the mouse movement
	MouseClass* mouse;
	POINT mousePos = {};
	HWND* window;
	RECT rect;
	POINT ul;
	POINT lr;
	MSG* msg;

public:
	Game(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, MouseClass& mouse, HWND& window);
	virtual ~Game() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

