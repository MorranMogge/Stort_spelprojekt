#pragma once
#include "GPU.h"
#include "DepthStencilView.h"

class RenderTargetView final
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> data;

public:

	float backgroundColor[4]{ 0.0f, 0.0f, 0.0f, 1.0f };

	ID3D11RenderTargetView* Get()
	{
		return data.Get();
	}
	ID3D11RenderTargetView** GetAddressOf()
	{
		return data.GetAddressOf();
	}
	void CreateBackBuffer()
	{
		ID3D11Texture2D* backBuffer = nullptr;
		if (FAILED(GPU::swapChain->GetBuffer(
			0,//index of the buffer we wanna get, 0 means back buffer
			__uuidof(ID3D11Texture2D),//get the uniq id of the interface we want to receive on that sub object
			reinterpret_cast<void**>(&backBuffer))))//fill the pointer
		{
			return;
		}

		//use that resource to create render target view on that resource
		GPU::device->CreateRenderTargetView(
			backBuffer,// pass the back buffer resource, we want view on
			nullptr,//how to create that target view, pass null to use default one
			data.GetAddressOf());//pass the renderTargetView

		backBuffer->Release();
	}
	void Create(ID3D11Texture2D* texture2d, D3D11_TEXTURE2D_DESC& texDesc)
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = texDesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		GPU::device->CreateRenderTargetView(texture2d, &desc, data.GetAddressOf());
	}
	void Create(ID3D11Texture2D* texture2d, D3D11_RENDER_TARGET_VIEW_DESC& rtvDesc)
	{
		GPU::device->CreateRenderTargetView(texture2d, &rtvDesc, data.GetAddressOf());
	}
	void BindWithDepth(DepthStencilView& dsv)
	{
		GPU::immediateContext->OMSetRenderTargets(1, data.GetAddressOf(), dsv.Get());
	}
	void BindWithOutDepth()
	{
		GPU::immediateContext->OMSetRenderTargets(1, data.GetAddressOf(), nullptr);
	}
	void Clear()
	{
		GPU::immediateContext->ClearRenderTargetView(data.Get(), backgroundColor);
	}

	static void UnBind()
	{
		ID3D11RenderTargetView* nullRTV = nullptr;
		GPU::immediateContext->OMSetRenderTargets(1u, &nullRTV, nullptr);
	}
	static void UnBindAll()
	{
		ID3D11RenderTargetView* nullRtv[8]{};
		GPU::immediateContext->OMSetRenderTargets(8, nullRtv, nullptr);
	}

};