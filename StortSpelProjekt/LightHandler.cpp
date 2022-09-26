#include "LightHandler.h"


//------------------------------------------------------------------------------- SETUP FUNCTIONS -------------------------------------------------------------------------------

bool CreateLtBuffer(ID3D11Device* device, StructuredBuffer<LightStruct>& lightBuffer, std::vector<Light>& lights)
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
		float typ = (float)lights.at(i).getType();
		DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixTranspose(lights.at(i).getViewMatrix());
		XMStoreFloat4x4(&matrix, tempMatrix);

		//Create & push back struct
		LightStruct lightArray(pos, col, dir, ca, typ, matrix);
		structVector.push_back(lightArray);
	}

	lightBuffer.Initialize(GPU::device, GPU::immediateContext, structVector);
	lightBuffer.applyData();

	return true; //!FAILED(hr);
}


bool CreateDepthStencil(ID3D11Device* device, UINT width, UINT height, Microsoft::WRL::ComPtr<ID3D11Texture2D>& dsTexture, std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>& dsViews, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& shaderView, int nrOfLights)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};											//skapa svartvit textur som representerar djup i en scen
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1u;														//olika nivååer av kompression
	textureDesc.ArraySize = nrOfLights;												//en buffer
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// MapUsageTypless(usage);
	textureDesc.SampleDesc.Count = 1;												// defaultvärden
	textureDesc.SampleDesc.Quality = 0;												//Sample quality
	textureDesc.Usage = D3D11_USAGE_DEFAULT;										//säger hur den ska användas när vi kommer åt den ()
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;	//binda till depth stencil
	textureDesc.CPUAccessFlags = 0;													// angeom cpu ska läsa eller skriva (vi behöver varken eller så därför 0)
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
	cBuffDesc.ByteWidth = 16;													//size of buffer //Kolla senare funktion för att hitta närmaste multipel av 16 för int!
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

LightHandler::~LightHandler()
{
	for (int i = 0; i < boundingSphere.size(); i++)
	{
		if (boundingSphere.at(i) != nullptr)
		{
			delete boundingSphere.at(i);
		}
	}
}

// ------------------------------------------------------------------------------- FUNCTIONS -------------------------------------------------------------------------------

void LightHandler::addLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 UpDir, int type, float coneAngle)
{
	//Check if light limit has been reached
	if (this->lights.size() < this->LightCap)
	{
		//Skapa ljus
		this->lights.push_back(Light(color, position, direction, UpDir, coneAngle, type));


		//current light id
		int lightID = (int)this->lights.size();
		

		//For first light create buffer, else unmap/remap buffer with new info
		if (lightID == 1)
		{
			//Create structured buffer containing all light data
			if (!CreateLtBuffer(GPU::device, this->lightBuffer, this->lights))
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
		if (!CreateViewBuffer(GPU::device, tempBuffer, this->lights.at(lightID - 1)))
		{
			std::cerr << "error creating viewBuffer!" << std::endl;
		}
		this->viewBuffers.push_back(tempBuffer);

		//Create Debug Mesh
		this->boundingSphere.push_back(new GameObject("../Meshes/Cone", position, direction, lightID));//Id does nothing yet!
	}
	else
	{
		std::cout << "Light limit reached! failed creating light" << std::endl;
	}
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
		float typ = (float)lights.at(i).getType();
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
	int nrOfLights = (UINT)this->lights.size();

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
	
	//Matrix update
	updateViewMatrix(lightIndex);

	//Mesh update
	this->boundingSphere.at(lightIndex)->setPos(position);
}

void LightHandler::setDirection(DirectX::XMFLOAT3 direction, int lightIndex)
{
	this->lights.at(lightIndex).setDirection(direction);

	//Matrix update
	updateViewMatrix(lightIndex);

	//Mesh update
	this->boundingSphere.at(lightIndex)->setRot(direction);
}

void LightHandler::setUpDirection(DirectX::XMFLOAT3 direction, int lightIndex)
{
	this->lights.at(lightIndex).setUpDirection(direction);

	//Matrix update
	updateViewMatrix(lightIndex);
}

void LightHandler::setColor(DirectX::XMFLOAT3 color, int lightIndex)
{
	this->lights.at(lightIndex).setColor(color);
}

void LightHandler::setConeAngle(float angle, int lightIndex)
{
	this->lights.at(lightIndex).setConeAngle(angle);
}

void LightHandler::setLightType(int type, int lightIndex)
{
	this->lights.at(lightIndex).setLightType(type);
}

void LightHandler::setRange(float range, int lightIndex)
{
	this->lights.at(lightIndex).setRange(range);
}

void LightHandler::setFalloff(float falloff, int lightIndex)
{
	this->lights.at(lightIndex).setFalloff(falloff);
}

bool LightHandler::updateViewMatrix(int lightIndex)
{
	//---------------------------------------- View Buffer ----------------------------------------

	//Update Debug meshes
	for (int i = 0; i < this->boundingSphere.size(); i++)
	{
		this->boundingSphere.at(i)->updateBuffer();
	}

	//Get matrix
	DirectX::XMMATRIX view = this->lights.at(lightIndex).getViewMatrix();

	//Map
	D3D11_MAPPED_SUBRESOURCE map;
	ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT hr = GPU::immediateContext->Map(this->viewBuffers.at(lightIndex).Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &view, sizeof(DirectX::XMMATRIX));

	//UnMap
	GPU::immediateContext->Unmap(this->viewBuffers.at(lightIndex).Get(), 0);
	return !FAILED(hr);
}

ID3D11Buffer* LightHandler::getViewBuffer(int ltIndex) const
{
	return this->viewBuffers.at(ltIndex).Get();
}

int LightHandler::getNrOfLights() const
{
	return (UINT)this->lights.size();
}

void LightHandler::drawShadows(int lightIndex, std::vector<GameObject*> gameObjects)
{
	//Variables
	ID3D11RenderTargetView* nullRtv{ nullptr };
	ID3D11DepthStencilView* nullDsView{ nullptr };

	//Set view buffer
	GPU::immediateContext->VSSetConstantBuffers(1, 1, this->viewBuffers.at(lightIndex).GetAddressOf());

	//unbind pixel shader
	//GPU::immediateContext->PSSetShader(nullptr, );

	//Clear Depth Stencil
	GPU::immediateContext->ClearDepthStencilView(this->depthViews.at(lightIndex).Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//Set render targets
	GPU::immediateContext->OMSetRenderTargets(1, &nullRtv, this->depthViews.at(lightIndex).Get());


	//Draw Objects
	for (int i = 0; i < gameObjects.size(); i++)	
	{
		gameObjects.at(i)->draw();
	}

	//Unbind render targets & Depth Stencil
	GPU::immediateContext->OMSetRenderTargets(1, &nullRtv, nullDsView);							
}

void LightHandler::bindLightBuffers()
{
	GPU::immediateContext->PSSetShaderResources(3, 1, this->shadowSrv.GetAddressOf());				//Bind Srv's //ShadowMap(s)
	this->lightBuffer.BindToPS(4);																	//Srv for light structuredBuffer content (pos, color, lightViewMatrix)
	GPU::immediateContext->PSSetConstantBuffers(0, 1, this->numLightBuffer.GetAddressOf());			//Bind CBuffers's //Buffer for nr Lights
}

void LightHandler::drawDebugMesh()
{
	for (int i = 0; i < this->boundingSphere.size(); i++)
	{
		this->boundingSphere.at(i)->draw();
	}
}

void LightHandler::unbindSrv()
{
	//Unbind shadowmap & structuredBuffer srv
	ID3D11ShaderResourceView* nullsrv{ nullptr };
	GPU::immediateContext->PSSetShaderResources(3, 1, &nullsrv);
	GPU::immediateContext->PSSetShaderResources(4, 1, &nullsrv);
}
