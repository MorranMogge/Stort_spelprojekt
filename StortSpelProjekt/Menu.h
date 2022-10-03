#pragma once
#define NOMINMAX
#include "State.h"
#include "GuiHandler.h"
#include "SettingsUI.h"
#include "Credits.h"
#include "D3D11Helper.h"




//Ny
#include "ModelManager.h"

class Menu : public State
{
private:

	GuiHandler startUI;
	SettingsUI settingsUI;
	Credits creditsUI;

	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	D3D11_VIEWPORT viewport;

	float clearColour[4]{ 0.0f,0.0f,0.0f,0.0f };

#pragma region Functions

	void UpdateConstanBuffer();

	void ShadowPass();
	void GeometryPass();

#pragma endregion

public:
	Menu();
	virtual ~Menu() override;

	// Inherited via State
	virtual GAMESTATE Update() override;
	virtual void Render() override;
	virtual void DrawUI() override;
};