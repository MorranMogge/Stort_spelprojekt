#include "GuiHandler.h"
#include "GPU.h"

std::unique_ptr<DirectX::SpriteBatch> GuiHandler::spriteBatch;

std::unique_ptr<GUISprite> GuiHandler::spriteWeaponBox;
std::unique_ptr<GUIText> GuiHandler::HPText;

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GuiHandler::spriteView;
Microsoft::WRL::ComPtr<ID3D11RasterizerState> GuiHandler::rss;
Microsoft::WRL::ComPtr<ID3D11BlendState> GuiHandler::blendState;
Microsoft::WRL::ComPtr<ID3D11DepthStencilState> GuiHandler::dss;
Microsoft::WRL::ComPtr<ID3D11SamplerState> GuiHandler::sampler;

void GuiHandler::SpritePass()
{
	spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState.Get(), sampler.Get(), dss.Get(), rss.Get());
	
	spriteWeaponBox->Draw(spriteBatch.get());

	spriteBatch->End();
	GPU::immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);
}

void GuiHandler::TextPass()
{


	spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState.Get(), sampler.Get(), dss.Get(), rss.Get());

	HPText->Draw(spriteBatch.get());

	spriteBatch->End();
	GPU::immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);
}

void GuiHandler::Init()
{

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(GPU::immediateContext);
	
	spriteWeaponBox = std::make_unique<GUISprite>(DirectX::SimpleMath::Vector2(300, 200), 0.0f);
	spriteWeaponBox->Load(GPU::device, L"../Sprites/WeaponBoxBone.dds");
	spriteWeaponBox->SetScale({ 0.5f,0.5f });

	HPText = std::make_unique<GUIText>(DirectX::SimpleMath::Vector2(500, 400), 0.0f);
	HPText->Load(GPU::device, L"../Sprites/GothenbergTextura.spritefont");
	HPText->setText(L"Test Text");
	HPText->SetTint(DirectX::Colors::Beige.v);


	D3D11_SAMPLER_DESC samplerDesc = {};
	//filter Nearst Neighbhor or Bilinear for 2D textures, linear get a smoother then zoom in to the texture
	//min is minification, mag is bigger, mip is mipmapping
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	GPU::device->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());


	D3D11_RASTERIZER_DESC desc = {};
	desc.AntialiasedLineEnable = false;

	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	GPU::device->CreateRasterizerState(&desc, rss.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC dssdesc = {};
	dssdesc.DepthEnable = true;
	dssdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dssdesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; //D3D11_COMPARISON_LESS //D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;//D3D11_COMPARISON_LESS_EQUAL
	GPU::device->CreateDepthStencilState(&dssdesc, dss.GetAddressOf());


	//SpriteBlendShape
	D3D11_BLEND_DESC blendDesc{};
	D3D11_RENDER_TARGET_BLEND_DESC& brt = blendDesc.RenderTarget[0];

	brt.BlendEnable = true;
	brt.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	brt.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	brt.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	brt.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	brt.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	brt.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL; //write all channels

	GPU::device->CreateBlendState(&blendDesc, blendState.GetAddressOf());
}

void GuiHandler::Update()
{
	SpritePass();
	TextPass();
}
