#pragma once
#include "Buffer.h"

class IndexBuffer: public Buffer
{
	bool is16bit = false;
public:

	IndexBuffer() {}
	IndexBuffer(const std::vector<unsigned short>& indices)
	{
		Create(indices);
	}
	IndexBuffer(const std::vector<unsigned int>& indices)
	{
		Create(indices);
	}

	void Create(const std::vector<unsigned short>& indices)
	{
		is16bit = true;
		// create index buffer
		D3D11_BUFFER_DESC inDesc{
			inDesc.ByteWidth = (unsigned int)(indices.size() * sizeof(unsigned short)),
			inDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
			inDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER,
			inDesc.CPUAccessFlags = 0,
			inDesc.MiscFlags = 0,
			inDesc.StructureByteStride = sizeof(unsigned short)
		};
		D3D11_SUBRESOURCE_DATA inSubRD{
			inSubRD.pSysMem = indices.data(),//pointer of the data
			inSubRD.SysMemPitch = 0,
			inSubRD.SysMemSlicePitch = 0
		};
		GPU::device->CreateBuffer(&inDesc, &inSubRD, data.GetAddressOf());
	}
	void Create(const std::vector<unsigned int>& indices)
	{
		is16bit = false;
		// create index buffer
		D3D11_BUFFER_DESC inDesc{
			inDesc.ByteWidth = (unsigned int)(indices.size() * sizeof(unsigned int)),
			inDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
			inDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER,
			inDesc.CPUAccessFlags = 0,
			inDesc.MiscFlags = 0,
			inDesc.StructureByteStride = sizeof(unsigned int)
		};
		D3D11_SUBRESOURCE_DATA inSubRD{
			inSubRD.pSysMem = indices.data(),//pointer of the data
			inSubRD.SysMemPitch = 0,
			inSubRD.SysMemSlicePitch = 0
		};
		GPU::device->CreateBuffer(&inDesc, &inSubRD, data.GetAddressOf());
	}

	void Bind() const
	{
		if (is16bit)
			GPU::immediateContext->IASetIndexBuffer(data.Get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
		else
			GPU::immediateContext->IASetIndexBuffer(data.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	}

};