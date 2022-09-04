#pragma once
#include "GPU.h"
class BlendState
{
	Microsoft::WRL::ComPtr <ID3D11BlendState> data;

public:

	void Create()
	{
		D3D11_BLEND_DESC desc{};
		D3D11_RENDER_TARGET_BLEND_DESC& brt = desc.RenderTarget[0];

		brt.BlendEnable = true;
		brt.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		brt.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		brt.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		brt.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

		brt.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		brt.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL; //write all channels
		GPU::device->CreateBlendState(&desc, data.GetAddressOf());
	}
	void Bind()
	{
		GPU::immediateContext->OMSetBlendState(data.Get(), nullptr, 0xffffffffu);
	}

};

