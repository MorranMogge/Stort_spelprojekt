#pragma once
#include <d3d11.h>
#include "D3D11Helper.h"
#include "ShaderLoader.h"

class BasicRenderer
{
private:
	float clearColour[4];

	ID3D11DeviceContext* immediateContext;

	ID3D11RenderTargetView* rtv;
	ID3D11DepthStencilView* dsView;
	ID3D11Texture2D* dsTexture;
	ID3D11SamplerState* sampler;

	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;

	ID3D11InputLayout* animLayout;
	ID3D11VertexShader* vShaderAnim;


	D3D11_VIEWPORT viewport;

	bool setUpInputLayout(ID3D11Device* device, const std::string& vShaderByteCode, ID3D11InputLayout* iLayout);
	bool setUpInputLayoutAnim(ID3D11Device* device, const std::string &vShaderByteCode, ID3D11InputLayout* iLayout);
	bool setUpSampler(ID3D11Device* device);

public:
	BasicRenderer();
	~BasicRenderer();
	
	void lightPrePass();
	bool initiateRenderer(ID3D11DeviceContext* immediateContext, ID3D11Device* device, IDXGISwapChain* swapChain, UINT WIDTH, UINT HEIGHT);
	void setUpScene();
	void changeToAnimation();
};