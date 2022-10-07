#pragma once
#include "stdafx.h"
#include "D3D11Helper.h"
#include "ShaderLoader.h"
#include "Camera.h"


class BasicRenderer
{
private:
	float clearColour[4];

	ID3D11DeviceContext* immediateContext;

	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsView;
	ID3D11DepthStencilView* dsView2;	//Used for binding as render target
	ID3D11Texture2D* dsTexture2;		//	 ----------||------------
	ID3D11ShaderResourceView* depthSrv;	
	ID3D11Texture2D* dsTexture;
	ID3D11SamplerState* sampler;
	ID3D11DepthStencilState* dsState;
	
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11PixelShader* ApShader;
	ID3D11VertexShader* vs_Skybox;
	ID3D11PixelShader* ps_Skybox;
	ID3D11InputLayout* inputLayout_Skybox;

	ID3D11InputLayout* pt_inputLayout;
	ID3D11VertexShader* pt_vShader;
	ID3D11PixelShader* pt_pShader;
	ID3D11ComputeShader* pt_UpdateShader;
	ID3D11GeometryShader* pt_gShader;

	D3D11_VIEWPORT viewport;

	bool setUpInputLayout(ID3D11Device* device, const std::string &vShaderByteCode);
	bool setUp_PT_InputLayout(ID3D11Device* device, const std::string& vShaderByteCode);
	bool setUp_Sky_InputLayout(ID3D11Device* device, const std::string& vShaderByteCode);
	bool setUpSampler(ID3D11Device* device);

public:
	BasicRenderer();
	~BasicRenderer();
	
	void lightPrePass();
	bool initiateRenderer(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, UINT WIDTH, UINT HEIGHT);
	void setUpScene(Camera& stageCamera);
	void geometryPass(Camera & stageCamera);
	void depthPrePass();
	void depthUnbind();
	void skyboxPrePass();
	void bindAmbientShader();
};
