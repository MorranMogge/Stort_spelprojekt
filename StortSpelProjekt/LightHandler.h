#pragma once
#include "Light.h"
#include <wrl.h>
#include <vector>
#include "ConstantBufferNew.h"
#include "StructuredBuffer.h"




class LightHandler
{
private:


	//Shadow

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadowSrv;					//Srv Shadow texture for lights		//Bind to shader to sample from shadow map
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTextures;						//Depth texture-array for lights
	std::vector<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> depthViews;		//Depth stencil for lights			//Bind to render to
	UINT shadowHeight;															//Width shadowmap
	UINT shadowWidth;															//Height shadowmap


	//Light

	std::vector<Light> lights;													//Vector of light objects
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> structuredBufferSrv;		//View for structured buffer
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> viewBuffers;				//View/proj matrix for each light
	Microsoft::WRL::ComPtr<ID3D11Buffer> numLightBuffer;						//Holds nr of lights

	//Mesh

	//std::vector<Mesh> boundingSphere;											//Mesh for visualization
	
public:

	LightHandler(UINT winWidth,UINT winHeight);
	void addLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 UpDir, int type = 0, float coneAngle = 0.5f);
	void finalizeLights(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	//bool updateLights(ID3D11DeviceContext* immediateContext, int ltIndex, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, float lightIntensity);
	

	ID3D11Buffer* getViewBuffer(int ltIndex) const;
	int getNrOfLights() const;
	
	/*
	void bindLightBuffer(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& cameraPosBuffer, ID3D11ShaderResourceView** dfSrvArr);
	void bindDepthStencil(ID3D11DeviceContext* immediateContext, int ptLightIndex);
	void bindAndDrawIndexedMesh(ID3D11DeviceContext* immediateContext, ID3D11Buffer*& viewProjBuffer, bool tesselation);
	*/
};