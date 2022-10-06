#include "Menu.h"
#include "Input.h"
#include "Console.h"
#include "SoundCollection.h"
#include "Topology.h"
#include "Shader.h"
#include "GameObject.h"
#include "LightHandler.h"
#include "GPU.h"


using namespace DirectX::SimpleMath;

Menu::Menu()
{

}

Menu::~Menu()
{

}

GAMESTATE Menu::Update()
{
	return ui.GetGameState();
}

void Menu::Render()
{
	ui.Draw();
}
