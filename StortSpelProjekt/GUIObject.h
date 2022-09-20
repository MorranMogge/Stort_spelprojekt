#pragma once

#include <SpriteBatch.h>
#include "DXUtil.h"
#include "GPU.h"
#include <SimpleMath.h>
#include <wrl.h>

class GUI final
{
	inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> spriteView;
	inline static Microsoft::WRL::ComPtr<ID3D11RasterizerState> rss;
	inline static Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
	inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dss;
	inline static Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	inline static bool init = false;
public:

	inline static std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

	static void Init()
	{
		if (init)
			return;

		spriteBatch = std::make_unique<DirectX::SpriteBatch>(GPU::immediateContext);

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
		init = true;
	}
	static void Begin()
	{
		spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, blendState.Get(), sampler.Get(), dss.Get(), rss.Get());
	}
	static void End()
	{
		spriteBatch->End();
		GPU::immediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFFu);
	}
};

class GUIObject
{
public:
	
	virtual ~GUIObject(){}

	//Virtual Methods

	//Load Funktion
	virtual void Load(ID3D11Device* device, const wchar_t* file) = 0; //(device, file,)


	//Get Funktions
	virtual const DirectX::SimpleMath::Vector2& GetPosition() const = 0;
	//virtual const DirectX::SimpleMath::Vector3& GetPosition() const = 0;
	virtual const DirectX::SimpleMath::Vector2& GetOrigin() const = 0;
	virtual const DirectX::SimpleMath::Vector2& GetScale() const = 0; //vector gives posibility to non-uniform scaling
	virtual const DirectX::SimpleMath::Color& GetTint() const = 0;
	virtual const float GetAlpha() const = 0;
	virtual const float GetRotation() const = 0;

	virtual const float GetWidth() const = 0;
	virtual const float GetHeight() const = 0;

	//Set Funktions
	virtual void SetPosition(const DirectX::SimpleMath::Vector2& position) = 0;
	virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin) = 0;
	virtual void SetScale(const DirectX::SimpleMath::Vector2& scale) = 0;
	virtual void SetTint(const DirectX::SimpleMath::Color& color) = 0;
	virtual void SetAlpha(const float alpha) = 0;
	virtual void SetRotation(const float rotation) = 0;

	//Render Funktion
	virtual void Draw() = 0;

};

