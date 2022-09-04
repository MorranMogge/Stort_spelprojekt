#pragma once
#include "GPU.h"

struct ViewPort : D3D11_VIEWPORT
{
	void Config(float width, float height)
	{
		Width = width;
		Height = height;
		MinDepth = 0.0f;
		MaxDepth = 1.0f;
		TopLeftX = 0.0f;
		TopLeftY = 0.0f;
	}
	void Bind()
	{
		GPU::immediateContext->RSSetViewports(1u, this);
	}

	static void UnBind()
	{
		GPU::immediateContext->RSSetViewports(0u, nullptr);
	}

};