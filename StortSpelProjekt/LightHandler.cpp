#include "LightHandler.h"


//------------------------------------------------------------------------------- SETUP FUNCTIONS -------------------------------------------------------------------------------

bool CreateLtBuffer(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Buffer>& lightBuffer, std::vector<Light>& lights, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& lightBuffView)
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



	StructuredBuffer<LightStruct>testStruct;
	for (int i = 0; i < lights.size(); i++)
	{
		testStruct.addData(structVector.at(i), device);
	}
	testStruct.applyData();


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

//------------------------------------------------------------------------------- CONSTRUCTOR -------------------------------------------------------------------------------


LightHandler::LightHandler(UINT winWidth, UINT winHeight)
	:shadowHeight(winHeight), shadowWidth(winWidth)
{
}

// ------------------------------------------------------------------------------- FUNCTIONS -------------------------------------------------------------------------------

void LightHandler::addLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 UpDir, int type, float coneAngle)
{
	//Skapa ljus
	Light templight( color, position, direction, UpDir, coneAngle, type);
	this->lights.push_back(templight);
}

void LightHandler::finalizeLights(ID3D11Device* device, ID3D11DeviceContext* immediateContext)
{

	//lägg till info från light i buffers 
	int nrOfLights = this->lights.size();

	////Create depth stencil, textureArr, depthViewArr & shader resource view
	//if (!CreateDepthStencil(device, this->shadowWidth, this->shadowHeight, this->depthTextures, this->depthViews, this->shaderView, nrOfLights))
	//{
	//	std::cerr << "error creating Dstencil/Srv!" << std::endl;
	//}

	//Create buffer containing projection & view Matrix data
	//for (int i = 0; i < nrOfLights; i++)
	//{
	//	ComPtr <ID3D11Buffer> tempBuffer;
	//	if (!CreateViewBuffer(device, tempBuffer, this->lights.at(i)))
	//	{
	//		std::cerr << "error creating viewBuffer!" << std::endl;
	//	}
	//	this->viewBuffers.push_back(tempBuffer);
	//}

	//Create single buffer containing all light data
	//if (!CreateLtBuffer(device, this->lightBuffer, this->Pointlights, this->lightBuffView))
	//{
	//	std::cerr << "error creating lightBuffer!" << std::endl;
	//}

	//Create buffer with int for nrof lights.
	//if (!CreateNumLTBuffer(device, this->numLightBuffer, nrOfLights))
	//{
	//	std::cerr << "error creating numlightBuffer!" << std::endl;
	//}

	//Create Meshes

}

ID3D11Buffer* LightHandler::getViewBuffer(int ltIndex) const
{
	return this->viewBuffers.at(ltIndex).Get();
}

int LightHandler::getNrOfLights() const
{
	return this->lights.size();
}
