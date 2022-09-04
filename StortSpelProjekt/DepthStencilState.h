#pragma once
#include "GPU.h"

class DepthStencilState
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> data;

public:

	void Create()
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL; //D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;//D3D11_COMPARISON_LESS_EQUAL
		GPU::device->CreateDepthStencilState(&desc, data.GetAddressOf());
	}
	void Bind()
	{
		GPU::immediateContext->OMSetDepthStencilState(data.Get(), 0u);
	}
	static void UnBind()
	{
		GPU::immediateContext->OMSetDepthStencilState(nullptr, 0);
	}

};