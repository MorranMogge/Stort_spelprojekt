#include "Menu.h"

Menu::Menu()
{
	std::cout << "Menu constructor \n";

	SoundCollection::testSound.play();

}

Menu::~Menu()
{

}

GAMESTATE Menu::Update()
{
	//return true ONCE key press
	if (Input::KeyPress(KeyCode::W))
	{
		std::cout << "W pressed \n";
	}

	//return true as long as key holding down
	if (Input::KeyDown(KeyCode::S))
	{
		std::cout << "S key is holding down \n";
	}

	//return true ONCE key release 
	if (Input::KeyUp(KeyCode::A))
	{
		std::cout << "A key released \n";
	}

	// destroy console window
	if (Input::KeyPress(KeyCode::Q))
	{
		Console::Destroy();
	}

	// exit game
	if (Input::KeyPress(KeyCode::ESC)) 
	{
		return GAMESTATE::EXIT;
	}

	return NOCHANGE;
}

void Menu::Render()
{
	ui.Draw();
}
