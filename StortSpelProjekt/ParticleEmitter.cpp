#include "ParticleEmitter.h"

bool CreateBuffer(ID3D11Device * device, ConstantBufferNew<particleStruct>  PT_vertexBuffer, ComPtr <ID3D11UnorderedAccessView>& particleUav, vector<particleStruct> structVector, XMFLOAT3 Pos)
{
	

	//D3D11_BUFFER_DESC bufferDesc;													//create Vertex buffer
	//bufferDesc.ByteWidth = PtData.size() * sizeof(XMFLOAT3);						//size of buffer
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;											//sets interaction with gpu and cpu
	//bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;	//Specifies the type of buffer
	//bufferDesc.CPUAccessFlags = 0;													//Specifies cpu acess
	//bufferDesc.MiscFlags = 0;														//Misc flags
	//bufferDesc.StructureByteStride = 0;												//Size of each element in structure
	//D3D11_SUBRESOURCE_DATA data;													//holds geometry data
	//data.pSysMem = PtData.data();													//pointer to geometry data
	//data.SysMemPitch = 0;															//distance in bytes from the beginning of one line of a texture to the next
	//data.SysMemSlicePitch = 0;														//for 3d texture




	PT_vertexBuffer.Initialize(GPU::device, GPU::immediateContext);
	PT_vertexBuffer.applyData();

	//Create Vtx buffer
	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, vertexBuffer.GetAddressOf());







	//Create UAV
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = { };
	uavDesc.Format = DXGI_FORMAT_R32_FLOAT;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = size(PtData) * 3;
	uavDesc.Buffer.Flags = 0;

	if (FAILED(device->CreateUnorderedAccessView(vertexBuffer.Get(), &uavDesc, particleUav.GetAddressOf())))
	{
		std::cerr << "failed to create UAV" << std::endl;
		return false;
	}
	return !FAILED(hr);
}

bool CreatePosActiveBuffer(ID3D11Device* device, ComPtr<ID3D11Buffer>& posBuffer, XMFLOAT3 position)
{
	XMFLOAT4 buff(position.x, position.y, position.z, 1);

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = sizeof(XMFLOAT4);										//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;													//Misc flags
	cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = &buff;													//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, posBuffer.GetAddressOf());
	return !FAILED(hr);
}

ParticleEmitter::ParticleEmitter(ID3D11Device * device, XMFLOAT3 Pos, int nrOfPT, XMFLOAT2 minMaxTime, int randRange)
	:Position(Pos), nrOfParticles(nrOfPT), active(true)
{
	//initilize particle data

	//Randomize particle positions in range
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

		this->PtData.at(i).pos = (XMFLOAT3(Pos.x + x, Pos.y + y, Pos.z + z));
		this->PtData.at(i).delta = 1;
		this->PtData.at(i).lifeTime = minMaxTime.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (minMaxTime.y - minMaxTime.x)));		
	}

	//Create particle vertex buffer & uav
	if (!CreateBuffer(device, this->PT_vertexBuffer, this->PT_UAV, this->PtData, Pos))
	{
		std::cerr << "error creating PtBuffer!" << std::endl;
	}

	//Create position buffer
	if (!CreatePosActiveBuffer(device, this->PT_posBuffer, Pos))
	{
		std::cerr << "error creating Pt Pos Buffer!" << std::endl;
	}
}


ID3D11Buffer * ParticleEmitter::getVTXBuffer()
{
	return this->PT_vertexBuffer.Get();
}

ID3D11Buffer* ParticleEmitter::getPosBuffer()
{
	return this->PT_posBuffer.Get();
}

void ParticleEmitter::BindAndDraw(ID3D11Buffer*& timeBuffer)
{




	//Variables
	vector<ID3D11Buffer*> tempBuff;
	tempBuff.push_back(cameraMatrix);
	tempBuff.push_back(cameraPosBuffer);

		////s?tt samma depth buffer fr?n geometripass.
		//immediateContext->VSSetShader(vShader, nullptr, 0);								//SetVTXShader
		//immediateContext->PSSetShader(pShader, nullptr, 0);								//Set PSShader
		//immediateContext->GSSetShader(geoShader, nullptr, 0);								//SetGeoShader
		//immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);		//Set how topology
		//immediateContext->IASetInputLayout(inputLayout);									//Input layout = float3 position for each vertex
		GPU::immediateContext->GSSetConstantBuffers(0, 2, tempBuff.data());					//Set camera pos for ,Set matrix [world],[view]
		GPU::immediateContext->OMSetRenderTargets(1, &rtv, dsView);							//SetRtv


	
		


	//Variables
	ID3D11GeometryShader* nullShader{ nullptr };
	ID3D11UnorderedAccessView* nullUav{ nullptr };
	ID3D11Buffer* nullBuffer{ nullptr };
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	int nrOfPt = this->nrOfParticles;


	//Draw
	GPU::immediateContext->IASetVertexBuffers(0, 1, this->PT_vertexBuffer.GetAddressOf(), &stride, &offset);		//Set VtxBuffer
	GPU::immediateContext->Draw(nrOfPt, 0);																		//Draw (once per primitive?)

	//Unbind UAV
	GPU::immediateContext->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);								//unbind vertex shader

	//Update positions on ComputeShader
	GPU::immediateContext->CSSetConstantBuffers(0, 1, &timeBuffer);												//Set ConstBuffer CS
	GPU::immediateContext->CSSetUnorderedAccessViews(0, 1, this->PT_UAV.GetAddressOf(), nullptr);				//Set UAV
	GPU::immediateContext->CSSetShader(particleUpdateShader, nullptr, 0);										//Set ComputeShader
	GPU::immediateContext->Dispatch(nrOfPt, 1, 1);																//Dispatch

	//Unbind shader & UAV, Reset Topology
	GPU::immediateContext->GSSetShader(nullShader, nullptr, 0);													//Unbinding
	GPU::immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);						//Reset Topology
	GPU::immediateContext->CSSetUnorderedAccessViews(0, 1, &nullUav, nullptr);									//Unbind UAV
}


ID3D11UnorderedAccessView * ParticleEmitter::getUAV()
{
	return this->PT_UAV.Get();
}

int ParticleEmitter::getNrOfParticles()
{
	return this->nrOfParticles;
}

void ParticleEmitter::updateBuffer(ID3D11DeviceContext* immediateContext)
{
	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));


	//Update buffer
	XMFLOAT4 baseMat = XMFLOAT4(this->Position.x, this->Position.y, this->Position.z, active);
	HRESULT hr = immediateContext->Map(this->PT_posBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &baseMat, sizeof(baseMat));

	//UnMap
	immediateContext->Unmap(this->PT_posBuffer.Get(), 0);
}

XMFLOAT3 ParticleEmitter::getPosition()
{
	return this->Position;
}

void ParticleEmitter::setPosition(XMFLOAT3 Pos)
{
	this->Position = Pos;
}

bool ParticleEmitter::isActive()
{
	return this->active;
}

void ParticleEmitter::setActive(bool onOrOff)
{
	this->active = onOrOff;
}
