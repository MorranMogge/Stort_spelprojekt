#include "stdafx.h"
#include "BasicRenderer.h"

bool BasicRenderer::setUpInputLayout(ID3D11Device* device, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, (UINT)std::size(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

	return !FAILED(hr);
}

bool BasicRenderer::setUp_Sky_InputLayout(ID3D11Device* device, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, (UINT)std::size(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout_Skybox);

	return !FAILED(hr);
}

bool BasicRenderer::setUp_PT_InputLayout(ID3D11Device* device, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[4] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"DELTA", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"START_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, (UINT)std::size(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &pt_inputLayout);

	return !FAILED(hr);
}

bool BasicRenderer::setUpSampler(ID3D11Device* device)
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &sampler);
	return !FAILED(hr);
}

bool BasicRenderer::setUpShadowSampler(ID3D11Device* device)
{
	//filter Nearst Neighbhor or Bilinear for 2D textures, linear get a smoother then zoom in to the texture
	//min is minification, mag is bigger, mip is mipmapping

	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
	desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	desc.MaxAnisotropy = 1;
	desc.BorderColor[0] = 1.0f;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;

	HRESULT hr = device->CreateSamplerState(&desc, &shadowSampler);
	return !FAILED(hr);
}

bool BasicRenderer::setUpShadowRastirizer(ID3D11Device* device)
{
	//control over how polygons are rendered, do things like render in wireframe or draw both the front and back faces. 

	D3D11_RASTERIZER_DESC desc = {};
	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 1.0f;
	desc.DepthClipEnable = true;
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;

	HRESULT hr = GPU::device->CreateRasterizerState(&desc, &shadowRastirizer);
	return !FAILED(hr);
}

BasicRenderer::BasicRenderer()
	:clearColour{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

BasicRenderer::~BasicRenderer()
{
	rtv->Release();
	dsView->Release();
	dsView2->Release();			//Used for binding as render target
	dsTexture2->Release();		//	 ----------||------------
	depthSrv->Release();
	dsTexture->Release();
	sampler->Release();
	dsState->Release();

	inputLayout->Release();
	vShader->Release();
	pShader->Release();
	ApShader->Release();
	vs_Skybox->Release();
	ps_Skybox->Release();
	inputLayout_Skybox->Release();

	pt_inputLayout->Release();
	pt_vShader->Release();
	pt_pShader->Release();
	pt_UpdateShader->Release();
	pt_gShader->Release();
	shadowSampler->Release();
	shadowRastirizer->Release();
	bill_vShader->Release();
	bill_pShader->Release();
	bill_gShader->Release();
}

void BasicRenderer::lightPrePass()
{
	ID3D11PixelShader* nullShader(nullptr);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(this->inputLayout);
	immediateContext->RSSetViewports(1, &shadowViewport);
	
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->PSSetShader(nullShader, nullptr, 0);
}

bool BasicRenderer::initiateRenderer(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, UINT WIDTH, UINT HEIGHT)
{
	int WidthAndHeight = 1024;

	std::string vShaderByteCode;
	this->immediateContext = immediateContext;

	if (this->immediateContext == nullptr)													return false;
	if (!CreateRenderTargetView(device, swapChain, rtv))									return false;
	if (!CreateDepthStencil(device, WIDTH, HEIGHT, dsTexture, dsView))						return false;
	if (!CreateDSState(dsState))															return false;
	if (!CreateDepthStencilAndSrv(device, WIDTH, HEIGHT, dsTexture2, dsView2, depthSrv))	return false;
	if (!LoadVertexShader(device, vShader, vShaderByteCode, "VertexShader"))				return false;
	if (!setUpInputLayout(device, vShaderByteCode))											return false;
	if (!LoadPixelShader(device, pShader, "PixelShader"))									return false;
	if (!LoadPixelShader(device, ApShader, "AmbientPixelShader"))							return false;
	if (!LoadVertexShader(device, pt_vShader, vShaderByteCode, "PT_VertexShader"))			return false;
	if (!setUp_PT_InputLayout(device, vShaderByteCode))										return false;
	if (!LoadPixelShader(device, pt_pShader, "PT_PixelShader"))								return false;
	if (!LoadGeometryShader(device, pt_gShader, "PT_GeometryShader"))						return false;
	if (!LoadComputeShader(device, pt_UpdateShader, "PT_UpdateShader"))						return false;
	if (!setUpSampler(device))																return false;
	if (!setUpShadowSampler(device))														return false;
	if (!setUpShadowRastirizer(device))   													return false;
	if (!LoadVertexShader(device, vs_Skybox, vShaderByteCode, "Skybox_VS"))					return false;
	if (!setUp_Sky_InputLayout(device, vShaderByteCode))									return false;
	if (!LoadPixelShader(device, ps_Skybox, "Skybox_PS"))									return false;
	if (!LoadVertexShader(device, bill_vShader, vShaderByteCode, "Bilboard_VS"))			return false;
	if (!LoadPixelShader(device, bill_pShader, "Bilboard_PS"))								return false;
	if (!LoadGeometryShader(device, bill_gShader, "Bilboard_GS"))							return false;

	

	SetViewport(viewport, WIDTH, HEIGHT);
	SetViewport(shadowViewport, WidthAndHeight, WidthAndHeight);
	
	return true;
}


void BasicRenderer::setUpScene()
{

	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->RSSetState(shadowRastirizer);
	immediateContext->PSSetSamplers(1, 1 ,&shadowSampler);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampler);

	//Unbind shadowmap & structuredBuffer srv
	ID3D11ShaderResourceView* nullRsv{ nullptr };
	immediateContext->PSSetShaderResources(3, 1, &nullRsv);
	immediateContext->PSSetShaderResources(4, 1, &nullRsv);
}

void BasicRenderer::setUpScene(Camera& stageCamera)
{				
	immediateContext->ClearRenderTargetView(rtv, clearColour);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->PSSetShaderResources(5, 1, &depthSrv);
	stageCamera.PSbindPositionBuffer(1);
	stageCamera.VSbindViewBuffer(1);
}

void BasicRenderer::geometryPass(Camera& stageCamera)
{
	//re-use same depth buffer as geometry pass.
	immediateContext->CSSetShader(pt_UpdateShader, nullptr, 0);							//Set ComputeShader
	immediateContext->VSSetShader(pt_vShader, nullptr, 0);								//SetVTXShader
	immediateContext->PSSetShader(pt_pShader, nullptr, 0);								//Set PSShader
	immediateContext->GSSetShader(pt_gShader, nullptr, 0);								//SetGeoShader
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);		//Set how topology
	immediateContext->IASetInputLayout(pt_inputLayout);									//Input layout = float3 position for each vertex
	stageCamera.GSbindViewBuffer(0);													//Set matrix [world],[view]
	stageCamera.GSbindPositionBuffer(1);												//Set camera pos for 
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);								//SetRtv
}

void BasicRenderer::depthPrePass()
{
	ID3D11RenderTargetView* nullRtv{ nullptr };
	immediateContext->ClearDepthStencilView(dsView2, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	immediateContext->OMSetRenderTargets(1, &rtv, dsView2);
}

void BasicRenderer::depthUnbind()
{
	GPU::immediateContext->OMSetDepthStencilState(nullptr, 0);
	immediateContext->VSSetShader(vShader, nullptr, 0);
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->IASetInputLayout(inputLayout);
}

void BasicRenderer::skyboxPrePass()
{
	immediateContext->OMSetDepthStencilState(dsState, 0);
	immediateContext->IASetInputLayout(inputLayout_Skybox);									
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);
	immediateContext->VSSetShader(vs_Skybox, nullptr, 0);								//SetVTXShader
	immediateContext->PSSetShader(ps_Skybox, nullptr, 0);								//Set PSShader
}

void BasicRenderer::bilboardPrePass(Camera& stageCamera)
{
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	immediateContext->IASetInputLayout(inputLayout_Skybox);
	immediateContext->VSSetShader(bill_vShader, nullptr, 0);
	immediateContext->PSSetShader(bill_pShader, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->GSSetShader(bill_gShader, nullptr, 0);
	stageCamera.GSbindViewBuffer(1);													//Set matrix [world],[view]
	stageCamera.GSbindPositionBuffer(2);												//Set camera pos for 
	immediateContext->OMSetRenderTargets(1, &rtv, dsView);								//SetRtv
}

void BasicRenderer::bindAmbientShader()
{
	immediateContext->PSSetShader(ApShader, nullptr, 0);
}