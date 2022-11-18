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
	//auto state = gamePad->GetState(0);

	a.Draw();
	b.Draw();
	x.Draw();
	y.Draw();
	arrow.Draw();

	//if (!state.IsConnected())
	//{

	//}
	//else
	//{
	//	useText.Draw();
	//	throwText.Draw();
	//	pickText.Draw();
	//	control.Draw();
	//}

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

#define scale 0.5f, 0.5f

	a = GUISprite(322 + left, 100 + upp);
	a.Load(GPU::device, L"../Sprites/control/a.png");
	a.SetScale(scale);

	b = GUISprite(322 + left, 175 + upp);
	b.Load(GPU::device, L"../Sprites/control/b.png");
	b.SetScale(scale);

	y = GUISprite(322 + left, 250 + upp);
	y.Load(GPU::device, L"../Sprites/control/y.png");
	y.SetScale(scale);

	x = GUISprite(322 + left, 325 + upp);
	x.Load(GPU::device, L"../Sprites/control/x.png");
	x.SetScale(scale);

	arrow = GUISprite(322 + left, 450 + upp);
	arrow.Load(GPU::device, L"../Sprites/control/Arrow.png");
	arrow.SetScale(0.3f, 0.3f);

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