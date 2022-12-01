#pragma once

#define NOMINMAX
#include "stdafx.h"

bool CreateInterfaces(const UINT& width, const UINT& height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain);

bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView*& rtv);

bool CreateDepthStencil(ID3D11Device* device, const UINT& width, const UINT& height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView);

bool CreateDepthStencilAndSrv(ID3D11Device* device, const UINT& width, const UINT& height, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, ID3D11ShaderResourceView*& shaderResource);

bool CreateDSState(ID3D11DepthStencilState*& dss);

bool CreatePT_DSState(ID3D11DepthStencilState*& dss);

bool CreateUnorderedView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11UnorderedAccessView*& uav);

void SetViewport(D3D11_VIEWPORT& viewport, const UINT &width, const UINT& height);

bool SetupD3D11(const UINT& width, const UINT& height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain,
	ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& dsTexture, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport);