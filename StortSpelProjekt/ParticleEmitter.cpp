#include "ParticleEmitter.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft;
using namespace WRL;

bool CreateBuffer(ComPtr<ID3D11Buffer>&  PT_vertexBuffer, ComPtr <ID3D11UnorderedAccessView>& particleUav, vector<particleStruct>& structVector)
{
	D3D11_BUFFER_DESC bufferDesc;													//create Vertex buffer
	bufferDesc.ByteWidth = structVector.size() * sizeof(particleStruct);			//size of buffer
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;											//sets interaction with gpu and cpu
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;	//Specifies the type of buffer
	bufferDesc.CPUAccessFlags = 0;													//Specifies cpu acess
	bufferDesc.MiscFlags = 0;														//Misc flags
	bufferDesc.StructureByteStride = 0;												//Size of each element in structure
	D3D11_SUBRESOURCE_DATA data;													//holds geometry data
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
	uavDesc.Buffer.NumElements = size(structVector) * 8;//fel???
	uavDesc.Buffer.Flags = 0;
	
	if (FAILED(GPU::device->CreateUnorderedAccessView(PT_vertexBuffer.Get(), &uavDesc, particleUav.GetAddressOf())))
	{
		std::cerr << "failed to create UAV" << std::endl;
		return false;
	}
	return !FAILED(hr);
}

bool CreatePosActiveBuffer(ComPtr<ID3D11Buffer>& posBuffer, XMFLOAT3 position, XMFLOAT3 rotation)
{
	vector<XMFLOAT4> data;
	data.push_back(XMFLOAT4(position.x, position.y, position.z, 1));
	data.push_back(XMFLOAT4(rotation.x, rotation.y, rotation.z, 0));

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = sizeof(XMFLOAT4) * data.size();						//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
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

bool CreateTimeBuffer(ComPtr<ID3D11Buffer>& timeBuffer, float &deltaTime)
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

ParticleEmitter::ParticleEmitter(XMFLOAT3 Pos, XMFLOAT3 Rot, int nrOfPT, XMFLOAT2 minMaxTime, int randRange)
	:Position(Pos), Rotation(Rot), nrOfParticles(nrOfPT), active(true), renderPassComplete(true)
{
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

		float lifeTime = 10;//minMaxTime.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (minMaxTime.y - minMaxTime.x)));
		//cout << "lifetime: " << lifeTime << endl;
		particleStruct tempStruct(XMFLOAT3(Pos.x + x, Pos.y + y, Pos.z + z), i, lifeTime);
		this->PT_Data.push_back(tempStruct);
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


ID3D11Buffer * ParticleEmitter::getVTXBuffer() const
{
	return this->PT_vertexBuffer.Get();
}

ID3D11Buffer* ParticleEmitter::getPosBuffer() const
{
	return this->emitterPosBuffer.Get();
}

void ParticleEmitter::BindAndDraw()
{
	//Variables
	ID3D11Buffer* nullBuffer{ nullptr };
	UINT stride = sizeof(particleStruct);
	UINT offset = 0;
	int nrOfPt = this->nrOfParticles;

	//Update delta time 
	this->updateTimeBuffer(tStruct.getDt());
	vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(this->timeBuffer.Get());
	tempBuff.push_back(this->emitterPosBuffer.Get());

	
	//Bind blendstate
	GPU::immediateContext->OMSetBlendState(this->blendState.Get(), nullptr, 0xffffffffu);
	
	//Draw
	GPU::immediateContext->IASetVertexBuffers(0, 1, this->PT_vertexBuffer.GetAddressOf(), &stride, &offset);	//Set VtxBuffer
	GPU::immediateContext->Draw(nrOfPt, 0);																		//Draw (once per primitive?)

	//Unbind UAV
	GPU::immediateContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);								//unbind vertex shader

	//Update positions on ComputeShader
	GPU::immediateContext->CSSetConstantBuffers(0, tempBuff.size(), tempBuff.data());							//Set ConstBuffers CS (time & emitter position)
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

void ParticleEmitter::unbind()
{
	//Variables
	ID3D11GeometryShader* nullShader{ nullptr };
	ID3D11UnorderedAccessView* nullUav{ nullptr };
	ID3D11BlendState* nullBlendstate{ nullptr };

	//Unbind shader & UAV, Reset Topology
	GPU::immediateContext->GSSetShader(nullShader, nullptr, 0);													//Unbinding
	GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);						//Reset Topology
	GPU::immediateContext->CSSetUnorderedAccessViews(0, 1, &nullUav, nullptr);									//Unbind UAV
	GPU::immediateContext->OMSetBlendState(nullBlendstate, nullptr, 0xffffffffu);								//Unbind blendstate
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
	vector<XMFLOAT4> data;
	data.push_back(XMFLOAT4(this->Position.x, this->Position.y, this->Position.z, this->active));
	data.push_back(XMFLOAT4(this->Rotation.x, this->Rotation.y, this->Rotation.z, 0));

	HRESULT hr = GPU::immediateContext->Map(this->emitterPosBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, data.data(), sizeof(XMFLOAT4) * data.size());

	//UnMap
	GPU::immediateContext->Unmap(this->emitterPosBuffer.Get(), 0);
}

void ParticleEmitter::updateTimeBuffer(float delta)
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

XMFLOAT3 ParticleEmitter::getPosition() const
{
	return this->Position;
}

DirectX::XMFLOAT3 ParticleEmitter::getRotation() const
{
	return this->Rotation;
}

void ParticleEmitter::setPosition(XMFLOAT3 Pos)
{
	this->Position = Pos;
}

void ParticleEmitter::setRotation(DirectX::XMFLOAT3 Rot)
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

void ParticleEmitter::setActive(bool onOrOff)
{
	this->active = onOrOff;
	if (onOrOff == false)
	{
		this->renderPassComplete = false;
	}
}

void ParticleEmitter::setPassComplete(bool onOrOff)
{
	this->renderPassComplete = onOrOff;
}
