#pragma once

#include "stdafx.h"
#include <wrl.h>

struct GPU final
{
	inline static ID3D11Device* device;
	inline static ID3D11DeviceContext* immediateContext;
	inline static IDXGISwapChain* swapChain;

	inline static unsigned int windowWidth;
	inline static unsigned int windowHeight;
};