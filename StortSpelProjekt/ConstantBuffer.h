#pragma once
#include "Buffer.h"

struct MatrixS
{
	DirectX::XMFLOAT4X4 matrix;
};
struct VectorS
{
	DirectX::SimpleMath::Vector4 vector;
};

class ConstantBuffer : public Buffer
{
public:

	ConstantBuffer(){}
	ConstantBuffer(const void* structData, const unsigned int structSize)
	{
		Create(structData, structSize);
	}

	void Create(const void* structData, const unsigned int structSize)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;

		//allows the use of the Lock funtion,it is also possible to update with UpdateSubresource on a non-dynamic, 
		//create once , then update it every frame
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		//give cpu acess to write
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = structSize;

		//to 0 because constant buffer isnt a array like the vertex buffer or index buffer
		//so dont need to know StructureByteStride,0 is fine for this 
		desc.StructureByteStride = 0;


		//it takes the pointer
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = structData;

		HRESULT hr = GPU::device->CreateBuffer(&desc, &subData, data.GetAddressOf());
	}

	void Update(const void* structData, const unsigned int structSize)
	{
		static D3D11_MAPPED_SUBRESOURCE map{};
		//RtlZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
		//memset(&map, 0, sizeof(D3D11_MAPPED_SUBRESOURCE));

		//disable GPU access to the buffer data
		GPU::immediateContext->Map(data.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &map);
		//update
		memcpy(map.pData, structData, structSize);

		//re enable GPU access to the buffer data
		GPU::immediateContext->Unmap(data.Get(), 0);
		
	}

	void BindToVS(const unsigned int index = 0) const
	{
		GPU::immediateContext->VSSetConstantBuffers(index, 1, data.GetAddressOf());
	}
	void BindToPS(const unsigned int index = 0) const
	{
		GPU::immediateContext->PSSetConstantBuffers(index, 1, data.GetAddressOf());
	}

	void BindToDS(const unsigned int index = 0) const
	{
		GPU::immediateContext->DSSetConstantBuffers(index, 1, data.GetAddressOf());
	}
	void BindToGS(const unsigned int index = 0) const
	{
		GPU::immediateContext->GSSetConstantBuffers(index, 1, data.GetAddressOf());
	}
	void BindToHS(const unsigned int index = 0) const
	{
		GPU::immediateContext->HSSetConstantBuffers(index, 1, data.GetAddressOf());
	}
	void BindToCS(const unsigned int index = 0) const
	{
		GPU::immediateContext->CSSetConstantBuffers(index, 1, data.GetAddressOf());
	}

};