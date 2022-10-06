#pragma once
#include "State.h"
#include "GuiHandler.h"
#include "Mesh.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "PerspectiveCamera.h"
#include "ShaderResourceView.h"
#include "DepthStencilView.h"
#include "BlendState.h"
#include "UnorderedAccessView.h"
#include "DepthStencilState.h"
#include "ViewPort.h"
#include "RenderTargetView.h"

class Menu : public State
{
private:

	GuiHandler ui;

public:
	Menu();
	~Menu();

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
};

