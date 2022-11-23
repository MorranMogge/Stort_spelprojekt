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

	auto state = gamePad->GetState(0);

	if (Input::KeyPress(KeyCode::MOUSE_L))
	{
		if (backText.GetTint() == DirectX::Colors::Green.v)
		{
			gameState = MENU;
		}
	}

	if (auto state = gamePad->GetState(0); state.IsConnected())
	{
		if (state.IsBPressed())
		{
			gameState = MENU;
		}
	}

}

void ControlUI::SpritePass()
{
	
	// if any gamepad connected
	if (auto state = gamePad->GetState(0); state.IsConnected())
	{
		a.Draw();
		b.Draw();
		x.Draw();
		y.Draw();
		arrow.Draw();

		usePickText.Draw();
		throwText2.Draw();
		moveText2.Draw();

		L_wheel.Draw();
		R_wheel.Draw();

		start.Draw();
	}

	//else show keyboard
	else
	{
		useText.Draw();
		throwText.Draw();
		pickText.Draw();
		control.Draw();
	}
	objective.Draw();
	backText.Draw();

}

GAMESTATE ControlUI::GetGameState()
{
	return gameState;
}

ControlUI::ControlUI()
{
	gamePad = std::make_unique<DirectX::GamePad>();

#define upp 60
#define left 80
	control = GUISprite(310 - left, 225 - upp);
	control.Load(L"../Sprites/control.png");
	control.SetScale(0.75, 0.75);

	useText = GUISprite(320 - left, 420 - upp);
	useText.Load(L"../Sprites/UseText.png");
	useText.SetScale(0.40f, 0.40f);

	throwText = GUISprite(340 - left, 500 - upp);
	throwText.Load(L"../Sprites/ThrowText.png");
	throwText.SetScale(0.40f, 0.40f);

	pickText = GUISprite(322 - left, 580 - upp);
	pickText.Load(L"../Sprites/PickText.png");
	pickText.SetScale(0.40f, 0.40f);

	backText = GUISprite(75 + 70, 600);
	backText.Load(L"../Sprites/backText.png");

#define scale 0.5f, 0.5f
#define offset2 100

	a = GUISprite(322 + left, 100 + upp - offset2);
	a.Load(L"../Sprites/control/a.png");
	a.SetScale(scale);

	b = GUISprite(322 + left, 175 + upp - offset2);
	b.Load(L"../Sprites/control/b.png");
	b.SetScale(scale);

	y = GUISprite(322 + left, 250 + upp - offset2);
	y.Load(L"../Sprites/control/y.png");
	y.SetScale(scale);

	x = GUISprite(322 + left, 325 + upp - offset2);
	x.Load(L"../Sprites/control/x.png");
	x.SetScale(scale);

	arrow = GUISprite(322 + left, 625 + upp - offset2);
	arrow.Load(L"../Sprites/control/Arrow.png");
	arrow.SetScale(0.3f, 0.3f);


	usePickText = GUISprite(322 + left + 200, 325 + upp - offset2);
	usePickText.Load(L"../Sprites/control/usepickText.png");
	usePickText.SetScale(0.5f, 0.5f);

	throwText2 = GUISprite(322 + left + 200, 175 + upp - offset2);
	throwText2.Load(L"../Sprites/control/throwText.png");
	throwText2.SetScale(0.5f, 0.5f);

	moveText2 = GUISprite(322 + left + 200, 450 + upp - offset2);
	moveText2.Load(L"../Sprites/control/moveText.png");
	moveText2.SetScale(0.5f, 0.5f);

	jumpText = GUISprite(322 + left + 200, 100 + upp - offset2);
	jumpText.Load(L"../Sprites/control/moveText.png");
	jumpText.SetScale(0.5f, 0.5f);

	L_wheel = GUISprite(322 + left, 450 + upp - offset2);
	L_wheel.Load(L"../Sprites/control/L_wheel.png");
	L_wheel.SetScale(0.5f, 0.5f);

	R_wheel = GUISprite(322 + left + 400, 450 + upp - offset2);
	R_wheel.Load(L"../Sprites/control/R_wheel.png");
	R_wheel.SetScale(0.5f, 0.5f);

	start = GUISprite(322 + left + 400, 625 + upp - offset2);
	start.Load(L"../Sprites/control/start.png");
	start.SetScale(0.5f, 0.5f);

	objective = GUISprite(310 - left, upp);
	objective.Load(L"../Sprites/Objective.png");
	objective.SetScale(0.75f, 0.75f);

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