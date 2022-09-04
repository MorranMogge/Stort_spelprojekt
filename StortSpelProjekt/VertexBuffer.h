#pragma once
#include "Buffer.h"
#include "Vertex.h"

class VertexBuffer : public Buffer
{
public:

	VertexBuffer() {}
	VertexBuffer(std::vector<Vertex>& vertices)
	{
		Create(vertices);
	}

	void Create(std::vector<Vertex>& vertices)
	{
		D3D11_BUFFER_DESC desc{
			desc.ByteWidth = (unsigned int)(sizeof(Vertex) * vertices.size()),
			desc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE,
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER,
			desc.CPUAccessFlags = 0,
			desc.MiscFlags = 0,
			desc.StructureByteStride = sizeof(Vertex)
		};
		D3D11_SUBRESOURCE_DATA subRD{
			subRD.pSysMem = vertices.data(),//pointer of the data
			subRD.SysMemPitch = 0,
			subRD.SysMemSlicePitch = 0
		};
		GPU::device->CreateBuffer(&desc, &subRD, data.GetAddressOf());
	}

	void Bind() const
	{
		const unsigned int stride = sizeof(Vertex);
		const unsigned int offset = 0u;

		GPU::immediateContext->IASetVertexBuffers(0, 1, data.GetAddressOf(), &stride, &offset);
	}

	static void UnBind()
	{
		GPU::immediateContext->IASetVertexBuffers(0, 0, nullptr, 0u, 0u);
	}

};