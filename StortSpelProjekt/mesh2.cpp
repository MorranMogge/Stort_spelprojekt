#include "mesh2.h"

HRESULT Mesh2::CreateIndexBuffer(std::vector<DWORD>& indexTriangle)
{
	
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * indexTriangle.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	size = indexTriangle.size();

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = indexTriangle.data();
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer);
	return hr;
}

HRESULT Mesh2::CreateVertexBuffer(std::vector<vertex>& vertexTriangle)
{

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(vertex) * vertexTriangle.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertexTriangle.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	return hr;
}

Mesh2::Mesh2(ID3D11Device* device, std::vector<vertex>& vertexTriangle, std::vector<DWORD>& indexTriangle)
{
	this->device = device;

	HRESULT hr = CreateVertexBuffer(vertexTriangle);
	if (FAILED(hr))
		ErrorLog::Log(hr, "Failed to create Vertex Buffer!");

	hr = CreateIndexBuffer(indexTriangle);
	if (FAILED(hr))
		ErrorLog::Log(hr, "Failed to create IndexBuffer!");
}

void Mesh2::Draw(ID3D11DeviceContext* immediateContext)
{
	UINT stride = sizeof(vertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	immediateContext->DrawIndexed(size, 0, 0);
}

Mesh2::Mesh2(const Mesh2& mesh)
{
	this->indexBuffer = mesh.indexBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->device = mesh.device;
	this->size = mesh.size;
}

ID3D11Buffer* Mesh2::getIndexBuffer() const
{
	return this->indexBuffer;
}

ID3D11Buffer* Mesh2::getVertexBuffer() const
{
	return this->vertexBuffer;
}

Mesh2::~Mesh2()
{
	vertexBuffer->Release();
	indexBuffer->Release();
}
