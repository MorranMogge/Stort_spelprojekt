#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <SimpleMath.h>

struct GPU final
{
	inline static ID3D11Device* device;
	inline static ID3D11DeviceContext* immediateContext;
	inline static IDXGISwapChain* swapChain;
};