#include "stdafx.h"
#include "BilboardObject.h"
#include "GPU.h"
#include "stb_image.h"

//----------------------------------------------- setUp Functions ------------------------------------------------//


bool CreateVTXBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer,const DirectX::XMFLOAT3 &position)
{
	D3D11_BUFFER_DESC bufferDesc;													//create Vertex buffer
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3);								//size of buffer
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;											//sets interaction with gpu and cpu
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;								//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;								//Specifies cpu acess
	bufferDesc.MiscFlags = 0;														//Misc flags
	bufferDesc.StructureByteStride = 0;												//Size of each element in structure
	D3D11_SUBRESOURCE_DATA data = {};												//holds geometry data
	data.pSysMem = &position;														//pointer to geometry data
	data.SysMemPitch = 0;															//distance in bytes from the beginning of one line of a texture to the next
	data.SysMemSlicePitch = 0;														//for 3d texture

	//Create Vtx buffer
	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, vertexBuffer.GetAddressOf());
	return !FAILED(hr);
}

bool CreateShaderResources(const std::vector<std::string>& filenames, std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>>& Textures, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& renderedTextureView)
{
	//variables
	std::vector<unsigned char*> images;
	int wth = 0;
	int hth = 0;
	int channels = 0;

	for (int i = 0; i < filenames.size(); i++)
	{
		unsigned char* img = stbi_load(("../Sprites/" + filenames.at(i)).c_str(), &wth, &hth, &channels, STBI_rgb_alpha);
		if (img == NULL)
		{
			return 0;
		}
		else
		{
			images.push_back(img);
		}
	}

	for (int i = 0; i < filenames.size(); i++)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = wth;
		textureDesc.Height = hth;
		textureDesc.MipLevels = 1u;
		textureDesc.ArraySize = 1u;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1u;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0u;
		textureDesc.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = images[i];
		data.SysMemPitch = wth * 4;
		data.SysMemSlicePitch = 0;


		//create texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D > tempTexture;
		if (FAILED(GPU::device->CreateTexture2D(&textureDesc, &data, tempTexture.GetAddressOf())))
		{
			std::cerr << "failed to create texture" << std::endl;
			return false;
		}
		Textures.push_back(tempTexture);

		//ShaderResource view 
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.TextureCube.MipLevels = 1;
		shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;

		//create shader resource view 
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tempSrv;
		if (FAILED(GPU::device->CreateShaderResourceView(Textures.at(i).Get(), &shaderResourceViewDesc, tempSrv.GetAddressOf())))
		{
			std::cerr << "failed to create texture view" << std::endl;
			return false;
		}
		renderedTextureView.push_back(tempSrv);
	}


	for (int i = 0; i < filenames.size(); i++)
	{
		stbi_image_free(images[i]);
	}

	return true;
}

//----------------------------------------------- Constructor ------------------------------------------------//

BilboardObject::BilboardObject(const std::vector<std::string>& textureNames, const DirectX::XMFLOAT3& position, const float & offset)
	:offset(offset), position(position)
{

	//Create texture & Srv
	if (!CreateShaderResources(textureNames, this->bilboardTX, this->bilboardTXView))
	{
		std::cout << "error creating texture & srv!" << std::endl;
	}
	
	//Create vertex buffer
	if (!CreateVTXBuffer(this->vertexBuffer, position))
	{
		std::cout << "error creating vtxBuffer!" << std::endl;
	}

}

BilboardObject::BilboardObject(const std::string& textureName, const DirectX::XMFLOAT3& position, const float& offset)
	:offset(offset), position(position)
{
	std::vector<std::string> tempStr{textureName};

	//Create texture & Srv
	if (!CreateShaderResources(tempStr, this->bilboardTX, this->bilboardTXView))
	{
		std::cout << "error creating lightBuffer!" << std::endl;
	}

	//Create vertex buffer
	if (!CreateVTXBuffer(this->vertexBuffer, position))
	{
		std::cout << "error creating lightBuffer!" << std::endl;
	}

}

//----------------------------------------------- Functions ------------------------------------------------//

void BilboardObject::setPosition(const DirectX::XMFLOAT3& position)
{
	this->position = position;
	this->updateBuffer();
}

DirectX::XMFLOAT3 BilboardObject::getPosition() const
{
	return this->position;
}

int BilboardObject::getNrOfTextures() const
{
	return (int)this->bilboardTX.size();
}

float BilboardObject::getOffset() const
{
	return this->offset;
}

void BilboardObject::setOffset(const float & offset)
{
	this->offset = offset;
}

void BilboardObject::PSbindSrv(const int& textureIndex, const int& slot)
{
	GPU::immediateContext->PSSetShaderResources(slot, 1, this->bilboardTXView.at(textureIndex).GetAddressOf());
}

void BilboardObject::VSbindVTXBuffer()
{
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	GPU::immediateContext->IASetVertexBuffers(0,1, this->vertexBuffer.GetAddressOf(), &stride, &offset);
}

void BilboardObject::bindAndDraw(const int& textureIndex, const int& slot)
{
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	GPU::immediateContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), &stride, &offset);
	GPU::immediateContext->PSSetShaderResources(slot, 1, this->bilboardTXView.at(textureIndex).GetAddressOf());
	GPU::immediateContext->Draw(1, 0);
}

void BilboardObject::updateBuffer()
{
	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//Update buffer
	HRESULT hr = GPU::immediateContext->Map(this->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &this->position, sizeof(DirectX::XMFLOAT3));

	//UnMap
	GPU::immediateContext->Unmap(this->vertexBuffer.Get(), 0);
}
