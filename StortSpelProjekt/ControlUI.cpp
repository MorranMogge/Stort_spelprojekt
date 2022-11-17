#include "stdafx.h"
#include "ControlUI.h"
#include "Input.h"
#include "GPU.h"

/*

                      ,-'`\
                  _,"'    j
           __....+       /               .
       ,-'"             /               ; `-._.'.
      /                (              ,'       .'
     |            _.    \             \   ---._ `-.
     ,|    ,   _.'  Y    \             `- ,'   \   `.`.
     l'    \ ,'._,\ `.    .              /       ,--. l
  .,-        `._  |  |    |              \       _   l .
 /              `"--'    /              .'       ``. |  )
.\    ,                 |                .        \ `. '
`.                .     |                '._  __   ;. \'
  `-..--------...'       \                  `'  `-"'.  \
      `......___          `._                        |  \
               /`            `..                     |   .
              /|                `-.                  |    L
             / |               \   `._               .    |
           ,'  |,-"-.   .       .     `.            /     |
         ,'    |     '   \      |       `.         /      |
       ,'     /|       \  .     |         .       /       |
     ,'      / |        \  .    +          \    ,'       .'
    .       .  |         \ |     \          \_,'        / j
    |       |  L          `|      .          `        ,' '
    |    _. |   \          /      |           .     .' ,'
    |   /  `|    \        .       |  /        |   ,' .'
    |   ,-..\     -.     ,        | /         |,.' ,'
    `. |___,`    /  `.   /`.       '          |  .'
      '-`-'     j     ` /."7-..../|          ,`-'
                |        .'  / _/_|          .
                `,       `"'/"'    \          `.
                  `,       '.       `.         |
             __,.-'         `.        \'       |
            /_,-'\          ,'        |        _.
             |___.---.   ,-'        .-':,-"`\,' .
                  L,.--"'           '-' |  ,' `-.\
                                        `.' 
*/


void ControlUI::HandleInputs()
{
	Input::Update();
	backText.IntersectMouse() ? backText.SetTint(DirectX::Colors::Green.v) : backText.SetTint(DirectX::Colors::White.v);

	if (Input::KeyPress(KeyCode::MOUSE_L))
	{
		if (backText.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = MENU;
		}
	}

}

void ControlUI::SpritePass()
{
	useText.Draw();
	throwText.Draw();
	pickText.Draw();
	control.Draw();

	backText.Draw();

}

GAMESTATE ControlUI::GetGameState()
{
	return gameState;
}

ControlUI::ControlUI()
{
#define upp 60
#define left 80
	control = GUISprite(310 - left, 225 - upp);
	control.Load(GPU::device, L"../Sprites/control.png");
	control.SetScale(0.75, 0.75);

	useText = GUISprite(320 - left, 420 - upp);
	useText.Load(GPU::device, L"../Sprites/UseText.png");
	useText.SetScale(0.40f, 0.40f);

	throwText = GUISprite(340 - left, 500 - upp);
	throwText.Load(GPU::device, L"../Sprites/ThrowText.png");
	throwText.SetScale(0.40f, 0.40f);

	pickText = GUISprite(322 - left, 580 - upp);
	pickText.Load(GPU::device, L"../Sprites/PickText.png");
	pickText.SetScale(0.40f, 0.40f);

	backText = GUISprite(75 + 70, 600);
	backText.Load(GPU::device, L"../Sprites/backText.png");

	gameState = NOCHANGE;
}

ControlUI::~ControlUI()
{

}

void ControlUI::Draw()
{
	GUI::Begin();
	HandleInputs();
	SpritePass();
	GUI::End();
}