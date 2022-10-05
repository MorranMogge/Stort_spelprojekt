#include "Menu.h"
#include "Input.h"
#include "Console.h"
#include "SoundCollection.h"
#include "Topology.h"
#include "GameObject.h"

void Menu::UpdateConstanBuffer()
{
}

void Menu::ShadowPass()
{

}

void Menu::GeometryPass()
{

}

Menu::Menu()
{
	std::cout << "Menu constructor \n";

	SoundCollection::testSound.play(); // play test sound

	//MaterialLibrary::LoadDefault(); //load default material

	CreateRenderTargetView(GPU::device, GPU::swapChain, rtv);
	CreateDepthStencil(GPU::device, GPU::windowWidth, GPU::windowHeight, dsTexture, dsView);
	SetViewport(viewport, GPU::windowWidth, GPU::windowHeight);
}

Menu::~Menu()
{
	rtv->Release();
	dsTexture->Release();
	dsView->Release();
}

GAMESTATE Menu::Update()
{
	return NOCHANGE;
}

void Menu::Render()
{
	GPU::immediateContext->ClearRenderTargetView(rtv, clearColour);
	GPU::immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	GPU::immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	GPU::immediateContext->RSSetViewports(1, &viewport);
//	UpdateConstanBuffer();
//
//	ShadowPass();
//	GeometryPass();
	DrawUI();
}

void Menu::DrawUI()
{

	if (GuiHandler::show)
		startUI.Draw();

	else if (SettingsUI::show)
		settingsUI.Draw();

	else if (Credits::show)
		creditsUI.Draw();
}