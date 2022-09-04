#include "GuiHandler.h"
#include "GPU.h"

using namespace DirectX::SimpleMath;

void GuiHandler::SpritePass()
{
	spriteWeaponBox.Draw();
}

void GuiHandler::TextPass()
{
	testText.Draw();
}

GuiHandler::GuiHandler()
{
	GUI::Init();

	spriteWeaponBox = GUISprite(300, 200);
	spriteWeaponBox.Load(GPU::device, L"../Sprites/WeaponBoxBone.dds");
	spriteWeaponBox.SetScale({ 0.5f,0.5f });

	testText = GUIText(500, 400);
	testText.Load(GPU::device, L"../Sprites/GothenbergTextura.spritefont");
	testText.setText(L"Test Text");
	testText.SetTint(DirectX::Colors::Beige.v);
}

void GuiHandler::Draw()
{
	GUI::Begin();
	SpritePass();
	TextPass();
	GUI::End();
}
