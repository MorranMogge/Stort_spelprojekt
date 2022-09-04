#pragma once
#include "GPU.h"

class RasterizerState
{
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> data;

public:

	enum class Cull
	{
		Back,
		Front,
		None,
	};
	inline void Create(const Cull cull, const bool show_WireFrame = false)
	{
		D3D11_RASTERIZER_DESC desc = {};

		//control over how polygons are rendered, do things like render in wireframe or draw both the front and back faces. 
		desc.AntialiasedLineEnable = false;

		switch (cull)
		{
		case Cull::Back:
			desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
			break;
		case Cull::Front:
			desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
			break;
		case Cull::None:
			desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
			break;
		}

		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;

		desc.FillMode = (show_WireFrame) ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;

		desc.FrontCounterClockwise = false;
		desc.MultisampleEnable = false;
		desc.ScissorEnable = false;
		desc.SlopeScaledDepthBias = 0.0f;

		GPU::device->CreateRasterizerState(&desc, &data);
	}
	inline void Bind()
	{
		GPU::immediateContext->RSSetState(data.Get());
	}

};