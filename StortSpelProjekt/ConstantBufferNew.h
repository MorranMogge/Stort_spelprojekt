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
	T bufferData;
public:
	ConstantBufferNew();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	ID3D11Buffer* const* getReferenceOf();
	ID3D11Buffer* Get()const;
	T &getData();
	ID3D11Buffer getBuffer();
	void applyData();
};


template<class T>
ConstantBufferNew<T>::ConstantBufferNew()
{
}

template<class T>
bool ConstantBufferNew<T>::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->deviceContext = deviceContext;

	D3D11_BUFFER_DESC bufferDesc = {};
	//bufferDesc.ByteWidth = sizeof(T);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(T);//static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	/*D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &bufferData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;*/

	//D3D11_SUBRESOURCE_DATA subData = {};
	//subData.pSysMem = (void*)&bufferData;

	HRESULT hr = device->CreateBuffer(&bufferDesc, 0u, buffer.GetAddressOf());
	if (FAILED(hr))
	{
		//ErrorLog::Log(hr, "Failed to create constantBuffer");
		return false;
	}
	return true;
}

template<class T>
ID3D11Buffer* const* ConstantBufferNew<T>::getReferenceOf()
{
	return buffer.GetAddressOf();
}

template<class T>
ID3D11Buffer* ConstantBufferNew<T>::Get() const
{
	return buffer.GetAddressOf();
}

template<class T>
T& ConstantBufferNew<T>::getData()
{
	return bufferData;
}

template<class T>
void ConstantBufferNew<T>::applyData()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		//ErrorLogger::Log(hr, "Failed to map constant buffer.");
		//return false;
	}
	CopyMemory(mappedResource.pData, &bufferData, sizeof(T));
	this->deviceContext->Unmap(buffer.Get(), 0);
	//return true;
}
