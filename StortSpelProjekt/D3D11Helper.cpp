#include "stdafx.h"
#include "D3D11Helper.h"
#include "GPU.h"

bool CreateInterfaces(const UINT& width, const UINT& height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Default
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT flags = 0;
	/*if (_DEBUG)
	{
		flags = D3D11_CREATE_DEVICE_DEBUG;
	}*/

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &immediateContext);

	GPU::device = device;
	GPU::immediateContext = immediateContext;
	GPU::swapChain = swapChain;


	//GPU::windowWidth = 1264;
	//GPU::windowHeight = 681;

	return !FAILED(hr);
}

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv)
{
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer!" << std::endl;
		return false;
	}

	HRESULT hr = device->CreateRenderTargetView(backBuffer, nullptr, &rtv);
	backBuffer->Release();
	return !FAILED(hr);
}

bool CreateDepthStencil(ID3D11Device* device, const UINT &width,const  UINT &height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "Failed to create depth stencil texture!" << std::endl;
		return false;
	}

	HRESULT hr = device->CreateDepthStencilView(dsTexture, nullptr, &dsView);
	return !FAILED(hr);
}

bool CreateDepthStencilAndSrv(ID3D11Device* device, const UINT& width, const UINT& height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, ID3D11ShaderResourceView*& shaderResource)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "Failed to create depth stencil texture!" << std::endl;
		return false;
	}

	//ShaderResource view 
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1u;

	//create shader resource view 
	HRESULT hr = device->CreateShaderResourceView(dsTexture, &shaderResourceViewDesc, &shaderResource);


	D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
	descView.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descView.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	descView.Flags = 0;
	descView.Texture2D.MipSlice = 0;

	HRESULT hr2 = device->CreateDepthStencilView(dsTexture, &descView, &dsView);
	return !FAILED(hr2);
}

bool CreateDSState(ID3D11DepthStencilState*&dss)
{
	D3D11_DEPTH_STENCIL_DESC dssdesc = {};
	dssdesc.DepthEnable = true;
	dssdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	dssdesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	HRESULT hr = GPU::device->CreateDepthStencilState(&dssdesc, &dss);

	return !FAILED(hr);
}

bool CreatePT_DSState(ID3D11DepthStencilState*& dss)
{
	D3D11_DEPTH_STENCIL_DESC dssdesc = {};
	dssdesc.DepthEnable = true;
	dssdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	dssdesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	dssdesc.StencilEnable = false;
	HRESULT hr = GPU::device->CreateDepthStencilState(&dssdesc, &dss);

	return !FAILED(hr);
}

void SetViewport(D3D11_VIEWPORT& viewport, const UINT& width, const UINT& height)
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool SetupD3D11(const UINT& width, const UINT& height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain, ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport)
{
	if (!CreateInterfaces(width, height, window, device, immediateContext, swapChain))
	{
		std::cerr << "Error creating interfaces!" << std::endl;
		return false;
	}

	if (!CreateRenderTargetView(device, swapChain, rtv))
	{
		std::cerr << "Error creating rtv!" << std::endl;
		return false;
	}

	if (!CreateDepthStencil(device, width, height, dsTexture, dsView))
	{
		std::cerr << "Error creating depth stencil view!" << std::endl;
		return false;
	}

	SetViewport(viewport, width, height);

	return true;
}
