#pragma once
#include "GPU.h"

class SamplerState
{
	Microsoft::WRL::ComPtr<ID3D11SamplerState> data;
	
public:

	enum class Mode { Wrap = 1, Clamp = 3, Border = 4, Mirror = 2, Mirror_Once = 5 };
	enum class Filter { Comparison_Linear = 149, Comparison_Point = 128, Linear = 21, Point = 0 };

	void Create(const Mode mode, const Filter filter)
	{
		D3D11_SAMPLER_DESC desc = {};
		//filter Nearst Neighbhor or Bilinear for 2D textures, linear get a smoother then zoom in to the texture
		//min is minification, mag is bigger, mip is mipmapping
		desc.Filter = (D3D11_FILTER)filter;

		desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)mode;
		desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)mode;
		desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)mode;

		//desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		GPU::device->CreateSamplerState(&desc, data.GetAddressOf());
	}
	void CreateShadow()
	{
		D3D11_SAMPLER_DESC desc = {};
		//filter Nearst Neighbhor or Bilinear for 2D textures, linear get a smoother then zoom in to the texture
		//min is minification, mag is bigger, mip is mipmapping
		desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;

		desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		desc.BorderColor[0] = 1.0f;
		//desc.BorderColor[1] = 1.0f;
		//desc.BorderColor[2] = 1.0f;
		//desc.BorderColor[3] = 1.0f;

		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		GPU::device->CreateSamplerState(&desc, data.GetAddressOf());
	}

	void BindToVS(const unsigned int index = 0u)
	{
		GPU::immediateContext->VSSetSamplers(index, 1, data.GetAddressOf());
	}
	void BindToPS(const unsigned int index = 0u)
	{
		GPU::immediateContext->PSSetSamplers(index, 1, data.GetAddressOf());
	}
	void BindToDS(const unsigned int index = 0u)
	{
		GPU::immediateContext->DSSetSamplers(index, 1, data.GetAddressOf());
	}
	void BindToGS(const unsigned int index = 0u)
	{
		GPU::immediateContext->GSSetSamplers(index, 1, data.GetAddressOf());
	}
	void BindToHS(const unsigned int index = 0u)
	{
		GPU::immediateContext->HSSetSamplers(index, 1, data.GetAddressOf());
	}
	void BindToCS(const unsigned int index = 0u)
	{
		GPU::immediateContext->CSSetSamplers(index, 1, data.GetAddressOf());
	}

};