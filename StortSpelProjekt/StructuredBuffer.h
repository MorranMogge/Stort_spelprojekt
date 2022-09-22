#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "BufferTypes.h"
#include <iostream>

template <class T>
class StructuredBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	ID3D11DeviceContext* deviceContext;
	std::vector<T> bufferData;

	bool reInitialize(ID3D11Device* device);
public:
	StructuredBuffer();
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<T>& buffData);
	ID3D11Buffer* const* getReferenceOf();
	ID3D11Buffer* Get()const;
	T& getData(int index);
	void addData(T& obj, ID3D11Device* device);
	void remapBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<T>& buffData);
	void applyData();

	void BindToVS(UINT toRegister);
	void BindToPS(UINT toRegister);
	void BindToHS(UINT toRegister);
	void BindToGS(UINT toRegister);
	void BindToDS(UINT toRegister);
	void BindToCS(UINT toRegister);
	ID3D11ShaderResourceView* getSRV();

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
		std::cout << "failed creating structured buffer" << std::endl;
		return false;
	}

	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.FirstElement = 0;
	shaderResourceViewDesc.Buffer.NumElements = bufferData.size();
	//create shader resource view 
	device->CreateShaderResourceView(buffer.Get(), &shaderResourceViewDesc, srv.GetAddressOf());

	return true;
}

template<class T>
StructuredBuffer<T>::StructuredBuffer()
{
}

template<class T>
bool StructuredBuffer<T>::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<T>& buffData)
{
	this->deviceContext = deviceContext;

	for (int i = 0; i < buffData.size(); i++)
	{
		bufferData.push_back(buffData.at(i));
	}

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = sizeof(T) * (UINT)bufferData.size();					//size of buffer //*nr of elements
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;				//Misc flags
	cBuffDesc.StructureByteStride = sizeof(T);									//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = bufferData.data();										//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, 0u, buffer.GetAddressOf());
	if (FAILED(hr))
	{
		//ErrorLog::Log(hr, "Failed to create constantBuffer");
		std::cout << "failed creating structured buffer" << std::endl;
		return false;
	}

	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.FirstElement = 0;
	shaderResourceViewDesc.Buffer.NumElements = (UINT)bufferData.size();

	//create shader resource view 
	HRESULT hr2 = device->CreateShaderResourceView(buffer.Get(), &shaderResourceViewDesc, srv.GetAddressOf());

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
	return buffer.Get();
}

template<class T>
T& StructuredBuffer<T>::getData(int index)
{
	return bufferData[index];
}

template<class T>
inline void StructuredBuffer<T>::addData(T& obj, ID3D11Device* device)
{
	buffer.~ComPtr();
	srv.~ComPtr();
	bufferData.push_back(obj);
	this->reInitialize(device);
}

template<class T>
inline void StructuredBuffer<T>::remapBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<T>& buffData)
{
	buffer.~ComPtr();
	srv.~ComPtr();
	bufferData.clear();
	for (int i = 0; i < buffData.size(); i++)
	{
		bufferData.push_back(buffData[i]);
	}
	this->Initialize(device, deviceContext, buffData);
}

template<class T>
void StructuredBuffer<T>::applyData()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
		//ErrorLogger::Log(hr, "Failed to map constant buffer.");
		std::cout << "Failed to map constant buffer." << std::endl;
		//return false;
	}
	CopyMemory(mappedResource.pData, &bufferData, sizeof(T));
	this->deviceContext->Unmap(buffer.Get(), 0);
	//return true;
}

template<class T>
inline void StructuredBuffer<T>::BindToVS(UINT toRegister)
{
	deviceContext->VSSetShaderResources(toRegister, 1, srv.GetAddressOf());
}

template<class T>
inline void StructuredBuffer<T>::BindToPS(UINT toRegister)
{
	deviceContext->PSSetShaderResources(toRegister, 1, srv.GetAddressOf());
}

template<class T>
inline void StructuredBuffer<T>::BindToHS(UINT toRegister)
{
	deviceContext->HSSetShaderResources(toRegister, 1, srv.GetAddressOf());
}

template<class T>
inline void StructuredBuffer<T>::BindToGS(UINT toRegister)
{
	deviceContext->GSSetShaderResources(toRegister, 1, srv.GetAddressOf());
}

template<class T>
inline void StructuredBuffer<T>::BindToDS(UINT toRegister)
{
	deviceContext->DSSetShaderResources(toRegister, 1, srv.GetAddressOf());
}

template<class T>
inline void StructuredBuffer<T>::BindToCS(UINT toRegister)
{
	deviceContext->CSSetShaderResources(toRegister, 1, srv.GetAddressOf()); deviceContext->VSSetShaderResources(toRegister, 1, srv.GetAddressOf());
}

template<class T>
inline ID3D11ShaderResourceView* StructuredBuffer<T>::getSRV()
{
	return srv.Get();
}