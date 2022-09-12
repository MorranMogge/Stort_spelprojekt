#include "BasicRenderer.h"

bool BasicRenderer::setUpInputLayout(ID3D11Device* device, const std::string& vShaderByteCode)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, std::size(inputDesc), vShaderByteCode.c_str(), vShaderByteCode.length(), &inputLayout);

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

BasicRenderer::BasicRenderer()
	:clearColour{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

BasicRenderer::~BasicRenderer()
{
	rtv->Release();
	dsView->Release();
	dsTexture->Release();
	sampler->Release();

	inputLayout->Release();
	vShader->Release();
	pShader->Release();
}

bool BasicRenderer::initiateRenderer(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, UINT WIDTH, UINT HEIGHT)
{
	std::string vShaderByteCode;
	this->immediateContext = immediateContext;
	if (this->immediateContext == nullptr)										return false;
	if (!CreateRenderTargetView(device, swapChain, rtv))						return false;
	if (!CreateDepthStencil(device, WIDTH, HEIGHT, dsTexture, dsView))			return false;
	if (!LoadVertexShader(device, vShader, vShaderByteCode, "VertexShader"))	return false;
	if (!LoadPixelShader(device, pShader, "PixelShader"))						return false;
	if (!setUpInputLayout(device, vShaderByteCode))								return false;
	if (!setUpSampler(device))		return false;
	SetViewport(viewport, WIDTH, HEIGHT);
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
	immediateContext->PSSetShader(pShader, nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampler);
}
