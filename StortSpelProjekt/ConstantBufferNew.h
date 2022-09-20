#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "BufferTypes.h"

template <class T>
class ConstantBufferNew
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext;
	T* bufferData;
public:
	ConstantBufferNew();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, void* data, UINT size);
	ID3D11Buffer* const* GetAddressOf();
	ID3D11Buffer* Get()const;
	T* getData();
	ID3D11Buffer getBuffer();
	void applyData(UINT size);
};


template<class T>
ConstantBufferNew<T>::ConstantBufferNew()
{
}

template<class T>
bool ConstantBufferNew<T>::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, void* data, UINT size)
{
	this->deviceContext = deviceContext;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_CPU_ACCESS_WRITE | D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = size;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = data;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &subData, buffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout <<"Failed to create constantBuffer" << std::endl;
		return false;
	}
	return true;
}

template<class T>
ID3D11Buffer* const* ConstantBufferNew<T>::GetAddressOf()
{
	return buffer.GetAddressOf();
}

template<class T>
ID3D11Buffer* ConstantBufferNew<T>::Get() const
{
	return buffer.Get();
}

template<class T>
T* ConstantBufferNew<T>::getData()
{
	return bufferData;
}

template<class T>
void ConstantBufferNew<T>::applyData(UINT size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		//ErrorLogger::Log(hr, "Failed to map constant buffer.");
		//return false;
	}
	CopyMemory(mappedResource.pData, bufferData, size);
	this->deviceContext->Unmap(buffer.Get(), 0);
	//return true;
}
