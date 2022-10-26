#include "stdafx.h"
#include "ParticleEmitter.h"
#include "stb_image.h"

//----------------------------------------------- setUp Functions ------------------------------------------------//

bool CreateBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>&  PT_vertexBuffer, Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>& particleUav, std::vector<particleStruct>& structVector)
{
	D3D11_BUFFER_DESC bufferDesc;													//create Vertex buffer
	bufferDesc.ByteWidth = (UINT)structVector.size() * sizeof(particleStruct);			//size of buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;											//sets interaction with gpu and cpu
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;	//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = 0;													//Specifies cpu acess
	bufferDesc.MiscFlags = 0;														//Misc flags
	bufferDesc.StructureByteStride = 0;												//Size of each element in structure
	D3D11_SUBRESOURCE_DATA data = {};													//holds geometry data
	data.pSysMem = structVector.data();												//pointer to geometry data
	data.SysMemPitch = 0;															//distance in bytes from the beginning of one line of a texture to the next
	data.SysMemSlicePitch = 0;														//for 3d texture

	//Create Vtx buffer
	HRESULT hr = GPU::device->CreateBuffer(&bufferDesc, &data, PT_vertexBuffer.GetAddressOf());

	//Create UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = (UINT)size(structVector) * 15;//fel???
	uavDesc.Buffer.Flags = 0;
	
	if (FAILED(GPU::device->CreateUnorderedAccessView(PT_vertexBuffer.Get(), &uavDesc, particleUav.GetAddressOf())))
	{
		std::cerr << "failed to create UAV" << std::endl;
		return false;
	}
	return !FAILED(hr);
}

bool CreatePosActiveBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& posBuffer, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation)
{
	std::vector<DirectX::XMFLOAT4> data;
	data.push_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1));
	data.push_back(DirectX::XMFLOAT4(rotation.x, rotation.y, rotation.z, 0));

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = (UINT)sizeof(DirectX::XMFLOAT4) * (UINT)data.size();						//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;													//Misc flags
	cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = data.data();												//pointer to data


	HRESULT hr = GPU::device->CreateBuffer(&cBuffDesc, &cBufData, posBuffer.GetAddressOf());
	return !FAILED(hr);
}

bool CreateTimeBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& timeBuffer, float &deltaTime)
{

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = 16;													//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;													//Misc flags
	cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = &deltaTime;												//pointer to data


	HRESULT hr = GPU::device->CreateBuffer(&cBuffDesc, &cBufData, timeBuffer.GetAddressOf());
	return !FAILED(hr);
}

bool CreateBlendState(Microsoft::WRL::ComPtr <ID3D11BlendState> &blendState)
{
	D3D11_BLEND_DESC desc{};
	D3D11_RENDER_TARGET_BLEND_DESC& brt = desc.RenderTarget[0];

	brt.BlendEnable = true;
	brt.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	brt.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	brt.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	brt.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

	brt.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	brt.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

	brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL; //write all channels
	HRESULT hr = GPU::device->CreateBlendState(&desc, blendState.GetAddressOf());

	return !FAILED(hr);
}

bool CreateShaderResource(const std::vector<std::string>& filenames, std::vector<Microsoft::WRL::ComPtr<ID3D11Texture2D>>& Textures, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& renderedTextureView)
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

ParticleEmitter::ParticleEmitter(const DirectX::XMFLOAT3& Pos, const DirectX::XMFLOAT3& Rot, const int& nrOfPT, const DirectX::XMFLOAT2& minMaxTime, int randRange)
	:Position(Pos), Rotation(Rot), nrOfParticles(nrOfPT), active(true), renderPassComplete(true), minMaxLifetime(minMaxTime)
{
	//particle types
	std::vector<std::string> textureNames{ "smoke.png", "player2.png" , "player3.png" , "player4.png" };

	//Initilize timer
	tStruct.startTime;

	//initilize particle data & Randomize particle positions in range
	srand((unsigned int)time(0));
	for (int i = 0; i < nrOfPT; i++)
	{
		int x = rand() % randRange + 1;
		int y = rand() % randRange + 1;
		int z = rand() % randRange + 1;

		int posX = rand() % 2;
		if (posX == 0)
		{
			x = -x;
		}
		int posY = rand() % 2;
		if (posY == 0)
		{
			y = -y;
		}
		int posZ = rand() % 2;
		if (posZ == 0)
		{
			z = -z;
		}

		float lifeTime = minMaxTime.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (minMaxTime.y - minMaxTime.x)));
		particleStruct tempStruct(DirectX::XMFLOAT3(/* + x*/0.0f , /*Pos.y + y*/0.0f, /*Pos.z + z*/0.0f ), DirectX::XMFLOAT3(Pos.x,Pos.y,Pos.z), (float)i, lifeTime, DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0, 0, 0));
		this->PT_Data.push_back(tempStruct);
	}


	//Create texture & Srv
	if (!CreateShaderResource(textureNames, this->PT_texture, this->PT_TXView))
	{
		std::cout << "error creating texture & srv!" << std::endl;
	}

	//Create particle vertex buffer & uav
	if (!CreateBuffer(this->PT_vertexBuffer, this->PT_UAV, this->PT_Data))
	{
		std::cerr << "error creating PT_Buffer!" << std::endl;
	}

	//Create position buffer
	if (!CreatePosActiveBuffer(this->emitterPosBuffer, Pos, Rot))
	{
		std::cerr << "error creating Emitter Pos Buffer!" << std::endl;
	}

	//Create time buffer
	float test = 1.0f;
	if (!CreateTimeBuffer(this->timeBuffer, test))
	{
		std::cerr << "error creating PT_time_Buffer!" << std::endl;
	}

	if (!CreateBlendState(this->blendState))
	{
		std::cerr << "error creating Blendstate!" << std::endl;
	}
}

//----------------------------------------------- Functions ------------------------------------------------//

ID3D11Buffer * ParticleEmitter::getVTXBuffer() const
{
	return this->PT_vertexBuffer.Get();
}

ID3D11Buffer* ParticleEmitter::getPosBuffer() const
{
	return this->emitterPosBuffer.Get();
}

void ParticleEmitter::BindAndDraw(int textureIndex)
{
	//Variables
	ID3D11Buffer* nullBuffer{ nullptr };
	UINT stride = sizeof(particleStruct);
	UINT offset = 0;
	int nrOfPt = this->nrOfParticles;

	//Update delta time 
	this->updateTimeBuffer(tStruct.getDt());
	std::vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(this->timeBuffer.Get());
	tempBuff.push_back(this->emitterPosBuffer.Get());

	
	//Bind blendstate
	GPU::immediateContext->OMSetBlendState(this->blendState.Get(), nullptr, 0xffffffffu);
	GPU::immediateContext->PSSetShaderResources(0, 1, this->PT_TXView.at(textureIndex).GetAddressOf());
	//Draw
	GPU::immediateContext->IASetVertexBuffers(0, 1, this->PT_vertexBuffer.GetAddressOf(), &stride, &offset);	//Set VtxBuffer
	GPU::immediateContext->Draw(nrOfPt, 0);																		//Draw once per primitive

	//Unbind UAV
	GPU::immediateContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);								//unbind vertex shader

	//Update positions on ComputeShader
	GPU::immediateContext->CSSetConstantBuffers(0, (UINT)tempBuff.size(), tempBuff.data());						//Set ConstBuffers CS (time & emitter position)
	GPU::immediateContext->CSSetUnorderedAccessViews(0, 1, this->PT_UAV.GetAddressOf(), nullptr);				//Set UAV
	GPU::immediateContext->Dispatch(nrOfPt, 1, 1);																//Dispatch

	//Reset delta time
	tStruct.resetStartTime();
	this->updateTimeBuffer(tStruct.getDt());

	//Reset render pass bool
	if (!this->renderPassComplete)
	{
		this->renderPassComplete = true;
	}
}




ID3D11UnorderedAccessView * ParticleEmitter::getUAV() const
{
	return this->PT_UAV.Get();
}

int ParticleEmitter::getNrOfParticles() const
{
	return this->nrOfParticles;
}

void ParticleEmitter::updateBuffer()
{
	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));


	//Update buffer
	std::vector<DirectX::XMFLOAT4> data;
	data.push_back(DirectX::XMFLOAT4(this->Position.x, this->Position.y, this->Position.z, this->active));
	data.push_back(DirectX::XMFLOAT4(this->Rotation.x, this->Rotation.y, this->Rotation.z, 0));

	HRESULT hr = GPU::immediateContext->Map(this->emitterPosBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, data.data(), sizeof(DirectX::XMFLOAT4) * data.size());

	//UnMap
	GPU::immediateContext->Unmap(this->emitterPosBuffer.Get(), 0);
}

void ParticleEmitter::updateTimeBuffer(const float &delta)
{
	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//Update buffer
	HRESULT hr = GPU::immediateContext->Map(this->timeBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &delta, 16);

	//UnMap
	GPU::immediateContext->Unmap(this->timeBuffer.Get(), 0);
}

DirectX::XMFLOAT3 ParticleEmitter::getPosition() const
{
	return this->Position;
}

DirectX::XMFLOAT3 ParticleEmitter::getRotation() const
{
	return this->Rotation;
}

void ParticleEmitter::setPosition(const DirectX::XMFLOAT3 &Pos)
{
	this->Position = Pos;
}

void ParticleEmitter::setRotation(const DirectX::XMFLOAT3 &Rot)
{
	this->Rotation = Rot;
}

bool ParticleEmitter::isActive()
{
	return this->active;
}

bool ParticleEmitter::isPassComplete()
{
	return this->renderPassComplete;
}

void ParticleEmitter::setActive(const bool &onOrOff)
{
	this->active = onOrOff;
	if (onOrOff == false)
	{
		this->renderPassComplete = false;
	}
}

void ParticleEmitter::setPassComplete(const bool &onOrOff)
{
	this->renderPassComplete = onOrOff;
}
