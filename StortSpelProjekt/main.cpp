#include "Game.h"
#include "Menu.h"

int main()
{
	State* currentState = new Menu();
	GAMESTATE stateInfo = NOCHANGE;

	while (stateInfo != EXIT)
	{
		stateInfo = currentState->Update();

		if (stateInfo != NOCHANGE)
		{
			switch (stateInfo)
			{
			case MENU:
				delete currentState;
				currentState = new Menu();
				break;
			case GAME:
				delete currentState;
				currentState = new Game();
				break;
			default:
				break;
			}
		}
	
		currentState->Render();
	}
	return 0;
}