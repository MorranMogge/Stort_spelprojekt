#pragma once
#include "GPU.h"
#include "Texture.h"

class DepthStencilView
{
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> data;

public:

	ID3D11DepthStencilView* Get()
	{
		return data.Get();
	}
	void Create(ID3D11Texture2D* texture2D, D3D11_DEPTH_STENCIL_VIEW_DESC& description)
	{
		GPU::device->CreateDepthStencilView(texture2D, &description, &data);
	}
	void Clear()
	{
		GPU::immediateContext->ClearDepthStencilView(data.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}
	void ClearDepth()
	{
		GPU::immediateContext->ClearDepthStencilView(data.Get(), D3D11_CLEAR_DEPTH | 0, 1, 0);
	}

};

