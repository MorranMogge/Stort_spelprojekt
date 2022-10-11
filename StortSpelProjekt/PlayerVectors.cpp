//#include "stdafx.h"
#include "PlayerVectors.h"
#include "GPU.h"
#include "ShaderLoader.h"
using namespace DirectX;

bool PlayerVectors::setUpShaders()
{
	std::string temp;
	LoadVertexShader(GPU::device, vShader, temp, "playerVectorVertex");
	LoadPixelShader(GPU::device, pShader, "plaverVectorPixel");
	return true;
}

bool PlayerVectors::setUpVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(Vertex) * 6;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	stride = sizeof(Vertex);
	offset = 0;

	D3D11_SUBRESOURCE_DATA data = {};

	data.pSysMem = &vectors;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, &vBuffer);

	return !FAILED(hr);
}

void PlayerVectors::updateVertexBuffer()
{
	
		vectors[0].position = vectors[1].position = vectors[2].position = 
			vectors[3].position = vectors[4].position = vectors[5].position = this->player->getPos();
		
		vectors[1].position += this->player->getForwardVec() * 50;
		vectors[3].position += this->player->getUpVec() * 50;
		vectors[5].position += this->player->getRightVec() * 50;

		D3D11_MAPPED_SUBRESOURCE resource;
		GPU::immediateContext->Map(vBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &vectors, 6 * sizeof(Vertex));
		GPU::immediateContext->Unmap(vBuffer, 0);
}

PlayerVectors::PlayerVectors()
{
	this->setUpShaders();
}

PlayerVectors::~PlayerVectors()
{
	if (vBuffer != nullptr) vBuffer->Release();
	vShader->Release();
	pShader->Release();
}

void PlayerVectors::setPlayer(Player* player)
{
	this->player = player;

	vectors[0].position = vectors[1].position = vectors[2].position =
		vectors[3].position = vectors[4].position = vectors[5].position = this->player->getPos();

	vectors[1].position += this->player->getForwardVec() * 100;
	vectors[3].position += this->player->getUpVec() * 100;
	vectors[5].position += this->player->getRightVec() * 100;

	
	vectors[0].normal = DirectX::SimpleMath::Vector3(1, 0, 0);
	vectors[1].normal = DirectX::SimpleMath::Vector3(1, 0, 0);
	vectors[2].normal = DirectX::SimpleMath::Vector3(0, 1, 0);
	vectors[3].normal = DirectX::SimpleMath::Vector3(0, 1, 0);
	vectors[4].normal = DirectX::SimpleMath::Vector3(0, 0, 1);
	vectors[5].normal = DirectX::SimpleMath::Vector3(0, 0, 1);


	this->setUpVertexBuffer();
}

void PlayerVectors::drawLines()
{
	this->updateVertexBuffer();
	GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	GPU::immediateContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	GPU::immediateContext->VSSetShader(vShader, nullptr, 0);
	GPU::immediateContext->PSSetShader(pShader, nullptr, 0);
	GPU::immediateContext->Draw(6, 0);
	GPU::immediateContext->PSSetShader(nullptr, nullptr, 0);
}
