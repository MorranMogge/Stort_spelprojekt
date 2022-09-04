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
	if (Input::KeyPress(KeyCode::W)) //return true ONCE key press
	{
		std::cout << "W pressed \n";
	}

	if (Input::KeyDown(KeyCode::S)) //return true as long as key holding down
	{
		std::cout << "S key is holding down \n";
	}

	if (Input::KeyUp(KeyCode::A)) //return true ONCE key release 
	{
		std::cout << "A key released \n";
	}

	return NOCHANGE;
}

void Menu::Render()
{
	ui.Draw();
}
