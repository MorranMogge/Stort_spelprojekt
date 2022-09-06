#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

GAMESTATE Game::Update()
{
	std::cout << "This is the Game State!\n";
	return NOCHANGE;
}

void Game::Render()
{
}