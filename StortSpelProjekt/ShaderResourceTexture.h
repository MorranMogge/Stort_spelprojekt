#pragma once
#include "GPU.h"
#include "Texture.h"

class ShaderResourceTexture
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

public:
	inline void Create(
		Texture& texture,
		const DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		const D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		const unsigned int bindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_TEXTURE2D_DESC texDesc{
			texDesc.Width = texture.width,
			texDesc.Height = texture.height,
			texDesc.MipLevels = 1,//not using mip mapping, 1
			texDesc.ArraySize = 1,//array of texture, only got one texture
			texDesc.Format = format,//DXGI_FORMAT_R8G8B8A8_UNORM;
			texDesc.SampleDesc.Count = 1,//
			texDesc.SampleDesc.Quality = 0,// no antiliasing
			texDesc.Usage = usage, //D3D11_USAGE::D3D11_USAGE_DEFAULT
			texDesc.BindFlags = bindFlags, //bind as resource, to pipiline to use by shader
			texDesc.CPUAccessFlags = 0,//no update from cpu, dont have cpu access flags, D3D11_CPU_ACCESS_FLAG 
			texDesc.MiscFlags = 0// no other flags D3D11_RESOURCE_MISC_FLAG
		};

		D3D11_SUBRESOURCE_DATA subRD{
			subRD.pSysMem = texture.color,
			subRD.SysMemPitch = texture.width * 4,
			subRD.SysMemSlicePitch = 0
		};

		if (FAILED(GPU::device->CreateTexture2D(&texDesc, &subRD, tex.GetAddressOf())))
		{
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = format;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (FAILED(GPU::device->CreateShaderResourceView(tex.Get(), &srvDesc, srv.GetAddressOf())))
		{
			return;
		}

	}

	inline void Create(
		const std::string texPath,
		const DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		const D3D11_USAGE usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
		const unsigned int bindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
	{
		Texture texture(texPath);
		Create(texture, format, usage, bindFlags);
	}

	void BindToVS(const unsigned int index = 0)
	{
		GPU::immediateContext->VSSetShaderResources(index, 1, srv.GetAddressOf());
	}
	void BindToHS(const unsigned int index = 0)
	{
		GPU::immediateContext->HSSetShaderResources(index, 1, srv.GetAddressOf());
	}
	void BindToDS(const unsigned int index = 0)
	{
		GPU::immediateContext->DSSetShaderResources(index, 1, srv.GetAddressOf());
	}
	void BindToGS(const unsigned int index = 0)
	{
		GPU::immediateContext->GSSetShaderResources(index, 1, srv.GetAddressOf());
	}
	void BindToPS(const unsigned int index = 0)
	{
		GPU::immediateContext->PSSetShaderResources(index, 1, srv.GetAddressOf());
	}
	void BindToCS(const unsigned int index = 0)
	{
		GPU::immediateContext->CSSetShaderResources(index, 1, srv.GetAddressOf());
	}

	ShaderResourceTexture() {}
	ShaderResourceTexture(const std::string loadPath)
	{
		Create(loadPath);
	}

};