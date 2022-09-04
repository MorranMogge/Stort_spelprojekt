#pragma once
#include "GUISprite.h"
#include "GUIText.h"
#include <d3d11.h>
#include <wrl.h>


class GuiHandler
{

private:

	static std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

	static std::unique_ptr<GUISprite> spriteWeaponBox;

	static std::unique_ptr<GUIText> HPText;

	static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> spriteView;
	static Microsoft::WRL::ComPtr<ID3D11RasterizerState> rss;
	static Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss;
	static Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;

	static void SpritePass();
	static void TextPass();
	
public:

	static void Init(); 
	static void Update();
	static void Release();
};

