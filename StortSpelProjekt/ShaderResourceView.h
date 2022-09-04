#pragma once

#include "GPU.h"

class ShaderResourceView
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> data;

public:

	ID3D11ShaderResourceView* Get()
	{
		return data.Get();
	}
	ID3D11ShaderResourceView** GetAddressOf()
	{
		return data.GetAddressOf();
	}
	void Create(ID3D11Texture2D* texture2D, D3D11_SHADER_RESOURCE_VIEW_DESC& description)
	{
		if (FAILED(GPU::device->CreateShaderResourceView(texture2D, &description, data.GetAddressOf())))
		{
			return;
		}
	}

	void BindToVS(const unsigned int index = 0u)
	{
		GPU::immediateContext->VSSetShaderResources(index, 1, data.GetAddressOf());
	}
	void BindToPS(const unsigned int index = 0u)
	{
		GPU::immediateContext->PSSetShaderResources(index, 1, data.GetAddressOf());
	}
	void BindToCS(const unsigned int index = 0u)
	{
		GPU::immediateContext->CSSetShaderResources(index, 1, data.GetAddressOf());
	}
	void BindToGS(const unsigned int index = 0u)
	{
		GPU::immediateContext->GSSetShaderResources(index, 1, data.GetAddressOf());
	}
	void BindToHS(const unsigned int index = 0u)
	{
		GPU::immediateContext->HSSetShaderResources(index, 1, data.GetAddressOf());
	}
	void BindToDS(const unsigned int index = 0u)
	{
		GPU::immediateContext->DSSetShaderResources(index, 1, data.GetAddressOf());
	}

	static void UnBindCS(const unsigned int numOf = 1u, const unsigned int startIndex = 0u)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		for (unsigned int i = startIndex; i < numOf; i++)
		{
			GPU::immediateContext->CSSetShaderResources(i, 1u, &nullSRV);
		}
	}
	static void UnBindPS(const unsigned int numOf = 1u, const unsigned int startIndex = 0u)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		for (unsigned int i = startIndex; i < numOf; i++)
		{
			GPU::immediateContext->PSSetShaderResources(i, 1u, &nullSRV);
		}
	}
	static void UnBindVS(const unsigned int numOf = 1u, const unsigned int startIndex = 0u)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		for (unsigned int i = startIndex; i < numOf; i++)
		{
			GPU::immediateContext->VSSetShaderResources(i, 1u, &nullSRV);
		}
	}
	static void UnBindHS(const unsigned int numOf = 1u, const unsigned int startIndex = 0u)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		for (unsigned int i = startIndex; i < numOf; i++)
		{
			GPU::immediateContext->HSSetShaderResources(i, 1u, &nullSRV);
		}
	}
	static void UnBindDS(const unsigned int numOf = 1u, const unsigned int startIndex = 0u)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		for (unsigned int i = startIndex; i < numOf; i++)
		{
			GPU::immediateContext->DSSetShaderResources(i, 1u, &nullSRV);
		}
	}
	static void UnBindGS(const unsigned int numOf = 1u, const unsigned int startIndex = 0u)
	{
		ID3D11ShaderResourceView* nullSRV = nullptr;
		for (unsigned int i = startIndex; i < numOf; i++)
		{
			GPU::immediateContext->GSSetShaderResources(i, 1u, &nullSRV);
		}
	}
};