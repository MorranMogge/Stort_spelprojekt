#pragma once
#include "GPU.h"

class UnorderedAccessView
{
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> data;

public:

	void Create(ID3D11Resource* resource)
	{
		GPU::device->CreateUnorderedAccessView(resource, nullptr, data.GetAddressOf());
	}
	void Create(ID3D11Resource* resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC& desc)
	{
		GPU::device->CreateUnorderedAccessView(resource, &desc, data.GetAddressOf());
	}

	void CreateBackBuffer()
	{
		ID3D11Texture2D* backBufferTex2D = nullptr;

		if (FAILED(GPU::swapChain->GetBuffer(
			0,//index of the buffer we wanna get, 0 means back buffer
			__uuidof(ID3D11Texture2D),//get the uniq id of the interface we want to receive on that sub object
			reinterpret_cast<void**>(&backBufferTex2D))))//fill the pointer
		{
			return;
		}

		//D3D11_UNORDERED_ACCESS_VIEW_DESC todo = {};
		//use that resource to create render target view on that resource
		GPU::device->CreateUnorderedAccessView(backBufferTex2D, nullptr, data.GetAddressOf());//pass the renderTargetView

		backBufferTex2D->Release();
	}

	void Bind(const unsigned int startSlot = 0u) const
	{
		GPU::immediateContext->CSSetUnorderedAccessViews(startSlot, 1, data.GetAddressOf(), nullptr);
	}
	void Dispatch(const unsigned int x, const unsigned int y, const unsigned int z) const
	{
		GPU::immediateContext->Dispatch(x, y, z);
	}
	static void UnBind(const unsigned int startSlot = 0u)
	{
		ID3D11UnorderedAccessView* nullUAV = nullptr;
		GPU::immediateContext->CSSetUnorderedAccessViews(startSlot, 1, &nullUAV, nullptr);
	}

};

