#include "LightHandler.h"


//------------------------------------------------------------------------------- SETUP FUNCTIONS -------------------------------------------------------------------------------

bool CreateLtBuffer(ID3D11Device* device, StructuredBuffer<LightStruct>lightBuffer, std::vector<Light>& lights, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& lightBuffView)
{
	std::vector<LightStruct> structVector;
	for (int i = 0; i < lights.size(); i++)
	{
		//Get data
		DirectX::XMFLOAT3 position = lights.at(i).getPosition();
		DirectX::XMFLOAT3 color = lights.at(i).getColor();
		DirectX::XMFLOAT3 direction = lights.at(i).getDirection();

		//Change to XMFLOAT4
		DirectX::XMFLOAT4X4 matrix;
		DirectX::XMFLOAT4 pos = DirectX::XMFLOAT4(position.x, position.y, position.z, 0);
		DirectX::XMFLOAT4 col = DirectX::XMFLOAT4(color.x, color.y, color.z, 0);
		DirectX::XMFLOAT4 dir = DirectX::XMFLOAT4(direction.x, direction.y, direction.z, 0);
		float ca = lights.at(i).getConeAngle();
		float typ = lights.at(i).getType();
		DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixTranspose(lights.at(i).getViewMatrix());
		XMStoreFloat4x4(&matrix, tempMatrix);

		//Create & push back struct
		LightStruct lightArray(pos, col, dir, ca, typ, matrix);
		structVector.push_back(lightArray);
	}


	lightBuffer.Initialize(GPU::device, GPU::immediateContext, structVector);
	lightBuffer.applyData();

	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	shaderResourceViewDesc.Buffer.FirstElement = 0;
	shaderResourceViewDesc.Buffer.NumElements = structVector.size();

	//create shader resource view 
	HRESULT hr = device->CreateShaderResourceView(lightBuffer.Get(), &shaderResourceViewDesc, lightBuffView.GetAddressOf());
	return !FAILED(hr);
}


bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, Microsoft::WRL::ComPtr<ID3D11Texture2D>& dsTexture, std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>& dsViews, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderView, int nrOfLights)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};											//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;														//olika niv��er av kompression
	textureDesc.ArraySize = nrOfLights;												//en buffer
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// MapUsageTypless(usage);
	textureDesc.SampleDesc.Count = 1;												// defaultv�rden
	textureDesc.SampleDesc.Quality = 0;												//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;										//s�ger hur den ska anv�ndas n�r vi kommer �t den ()
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;	//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;													// angeom cpu ska l�sa eller skriva (vi beh�ver varken eller s� d�rf�r 0)
	textureDesc.MiscFlags = 0;														//misc flagga


	//depth texture
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, dsTexture.GetAddressOf())))
	{
		std::cerr << "failed to create depth stencil texture" << std::endl;
		return false;
	}

	//DepthStencil views
	for (int i = 0; i < nrOfLights; i++)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
		descView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descView.Flags = 0;
		descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		descView.Texture2DArray.MipSlice = 0;
		descView.Texture2DArray.FirstArraySlice = i;
		descView.Texture2DArray.ArraySize = 1;


		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> tempDsView;
		//crate depth stencil, assign it to dsView
		if (FAILED(device->CreateDepthStencilView(dsTexture.Get(), &descView, tempDsView.GetAddressOf())))
		{
			std::cerr << "failed to create depth stencil texture" << std::endl;
			return false;
		}

		//push 
		dsViews.push_back(tempDsView);
	}


	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;//DXGI_FORMAT_R24G8_TYPELESS
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	shaderResourceViewDesc.Texture2DArray.ArraySize = nrOfLights;
	shaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
	shaderResourceViewDesc.Texture2DArray.MipLevels = 1u;
	shaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;


	//create shader resource view 
	HRESULT hr = device->CreateShaderResourceView(dsTexture.Get(), &shaderResourceViewDesc, shaderView.GetAddressOf());
	return !FAILED(hr);
}


bool CreateNumLTBuffer(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Buffer>& lightBuffer, int nrOfLights)
{
	int buff = nrOfLights;

	D3D11_BUFFER_DESC cBuffDesc = { 0 };
	cBuffDesc.ByteWidth = 16;													//size of buffer //Kolla senare funktion f�r att hitta n�rmaste multipel av 16 f�r int!
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;										//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;							//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;													//Misc flags
	cBuffDesc.StructureByteStride = 0;											//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData = { 0 };									//holds matrix data
	cBufData.pSysMem = &buff;													//pointer to data


	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, lightBuffer.GetAddressOf());
	return !FAILED(hr);
}


bool CreateViewBuffer(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Buffer>& viewBuffer, Light& light)
{
	DirectX::XMMATRIX view = { light.getViewMatrix() };

	D3D11_BUFFER_DESC cBuffDesc;
	cBuffDesc.ByteWidth = sizeof(DirectX::XMMATRIX);		//size of buffer
	cBuffDesc.Usage = D3D11_USAGE_DYNAMIC;					//sets interaction with gpu and cpu
	cBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//Specifies the type of buffer
	cBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		//Specifies cpu acess
	cBuffDesc.MiscFlags = 0;								//Misc flags
	cBuffDesc.StructureByteStride = 0;						//Size of each element in structure
	D3D11_SUBRESOURCE_DATA cBufData;						//holds matrix data
	cBufData.pSysMem = &view;								//pointer to geometry data

	HRESULT hr = device->CreateBuffer(&cBuffDesc, &cBufData, viewBuffer.GetAddressOf());
	return !FAILED(hr);
}
//------------------------------------------------------------------------------- CONSTRUCTOR -------------------------------------------------------------------------------


LightHandler::LightHandler()
	:shadowHeight(GPU::windowHeight), shadowWidth(GPU::windowWidth)
{

	//Create depth stencil, textureArr, depthViews & shader resource views 
	if (!CreateDepthStencil(GPU::device, this->shadowWidth, this->shadowHeight, this->depthTextures, this->depthViews, this->shadowSrv, this->LightCap))
	{
		std::cerr << "error creating Dstencil/Srv!" << std::endl;
	}
}

// ------------------------------------------------------------------------------- FUNCTIONS -------------------------------------------------------------------------------

void LightHandler::addLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 UpDir, int type, float coneAngle)
{
	//Skapa ljus
	this->lights.push_back(Light(color, position, direction, UpDir, coneAngle, type));
	

	//current light id
	int lightID = this->lights.size();


	//For first light create buffer, else unmap/remap buffer with new info
	if (lightID == 1)
	{
		//Create structured buffer containing all light data
		if (!CreateLtBuffer(GPU::device, this->lightBuffer, this->lights, this->structuredBufferSrv))
		{
			std::cout << "error creating lightBuffer!" << std::endl;
		}


		//Create buffer with int for nrof lights.
		if (!CreateNumLTBuffer(GPU::device, this->numLightBuffer, lightID))
		{
			std::cerr << "error creating numlightBuffer!" << std::endl;
		}

	}
	else
	{
		//Unmap structured buffer & numlight buffer
		this->updateBuffers();
	}

	//create& push back new viewMatrix buffer
	Microsoft::WRL::ComPtr <ID3D11Buffer> tempBuffer;
	if (!CreateViewBuffer(GPU::device, tempBuffer, this->lights.at(lightID-1)))
	{
		std::cerr << "error creating viewBuffer!" << std::endl;
	}
	this->viewBuffers.push_back(tempBuffer);




	////Create buffer containing view Matrix data



	//Create Meshes
}

bool LightHandler::updateBuffers()
{

	//---------------------------------------- Structured Buffer ----------------------------------------
	std::vector<LightStruct> structVector;
	for (int i = 0; i < lights.size(); i++)
	{
		//Get data
		DirectX::XMFLOAT3 position = lights.at(i).getPosition();
		DirectX::XMFLOAT3 color = lights.at(i).getColor();
		DirectX::XMFLOAT3 direction = lights.at(i).getDirection();

		//Change to XMFLOAT4
		DirectX::XMFLOAT4X4 matrix;
		DirectX::XMFLOAT4 pos = DirectX::XMFLOAT4(position.x, position.y, position.z, 0);
		DirectX::XMFLOAT4 col = DirectX::XMFLOAT4(color.x, color.y, color.z, 0);
		DirectX::XMFLOAT4 dir = DirectX::XMFLOAT4(direction.x, direction.y, direction.z, 0);
		float ca = lights.at(i).getConeAngle();
		float typ = lights.at(i).getType();
		DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixTranspose(lights.at(i).getViewMatrix());
		XMStoreFloat4x4(&matrix, tempMatrix);

		//Create & push back struct
		LightStruct lightArray(pos, col, dir, ca, typ, matrix);
		structVector.push_back(lightArray);
	}

	//Unmap/map Buffer
	this->lightBuffer.remapBuffer(GPU::device, GPU::immediateContext, structVector);
	this->lightBuffer.applyData();


	//---------------------------------------- NrLight Buffer ----------------------------------------

	//Ger nrOf lights
	int nrOfLights = this->lights.size();

	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT hr = GPU::immediateContext->Map(this->numLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &nrOfLights, 16); //kanske ger problem!!!

	//UnMap
	GPU::immediateContext->Unmap(this->numLightBuffer.Get(), 0);


	return !FAILED(hr);
}

void LightHandler::setPosition(DirectX::XMFLOAT3 position, int lightIndex)
{
	this->lights.at(lightIndex).setPosition(position);
	///UPDATE MATRIX!!!!
}

void LightHandler::setUpDirection(DirectX::XMFLOAT3 direction, int lightIndex)
{
	//this->lights.at(lightIndex).(direction);
	///UPDATE MATRIX!!!!
}

void LightHandler::setColor(DirectX::XMFLOAT3 color, int lightIndex)
{
	//this->lights.at(i).set
}

ID3D11Buffer* LightHandler::getViewBuffer(int ltIndex) const
{
	return this->viewBuffers.at(ltIndex).Get();
}

int LightHandler::getNrOfLights() const
{
	return this->lights.size();
}
