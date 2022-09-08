#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "BufferTypes.h"

template <class T>
class StructuredBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext;
	std::vector<T> bufferData;

	bool reInitialize(ID3D11Device* device);
public:
	StructuredBuffer();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	ID3D11Buffer* const* getReferenceOf();
	ID3D11Buffer* Get()const;
	T& getData(int index);
	void addData(T &obj, ID3D11Device* device);
	ID3D11Buffer getBuffer();
	void applyData();
};


template<class T>
inline bool StructuredBuffer<T>::reInitialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = sizeof(T) * bufferData.size();			//size of buffer //*nr of elements
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;				//Misc flags
	cBuffDesc.StructureByteStride = sizeof(T);						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = bufferData.data();										//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, 0u, buffer.GetAddressOf());
	if (FAILED(hr))
	{
		//ErrorLog::Log(hr, "Failed to create constantBuffer");
		return false;
	}
	return true;
}

template<class T>
StructuredBuffer<T>::StructuredBuffer()
{
}

template<class T>
bool StructuredBuffer<T>::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->deviceContext = deviceContext;

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = sizeof(T) * bufferData.size();			//size of buffer //*nr of elements
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;				//Misc flags
	cBuffDesc.StructureByteStride = sizeof(T);						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = bufferData.data();										//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, 0u, buffer.GetAddressOf());
	if (FAILED(hr))
	{
		//ErrorLog::Log(hr, "Failed to create constantBuffer");
		return false;
	}
	return true;
}

template<class T>
ID3D11Buffer* const* StructuredBuffer<T>::getReferenceOf()
{
	return buffer.GetAddressOf();
}

template<class T>
ID3D11Buffer* StructuredBuffer<T>::Get() const
{
	return buffer.GetAddressOf();
}

template<class T>
T& StructuredBuffer<T>::getData(int index)
{
	return bufferData[index];
}

template<class T>
inline void StructuredBuffer<T>::addData(T &obj, ID3D11Device* device)
{
	buffer.~ComPtr();
	bufferData.push_back(obj);
	this->reInitialize(device);
}

template<class T>
void StructuredBuffer<T>::applyData()
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
