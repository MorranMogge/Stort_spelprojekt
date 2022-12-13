#include "stdafx.h"
#include "InputIP.h"

InputIP::InputIP()
{
	basicRenderer.initiateRenderer(GPU::immediateContext, GPU::device, GPU::swapChain, GPU::windowWidth, GPU::windowHeight);
}

InputIP::~InputIP()
{

}

GAMESTATE InputIP::Update()
{
	return ui.GetGameState();
}

void InputIP::Render()
{
	basicRenderer.setUpScene();
	ui.Draw();
}
