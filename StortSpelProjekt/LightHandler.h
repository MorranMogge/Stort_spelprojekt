#pragma once
#include "Light.h"
#include <wrl.h>
#include <vector>
#include <iostream>
#include "ConstantBufferNew.h"
#include "StructuredBuffer.h"
#include "GPU.h"
#include "GameObject.h"


class LightHandler
{
private:


	//Shadow

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadowSrv;					//Shader view ShadowTx for lights	//Bind to shader to sample from shadow map
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTextures;						//Array of Depth textures
	std::vector < Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> depthViews;	//Depth stencil for lights			//Bind to render to
	UINT shadowHeight;															//Width shadowmap
	UINT shadowWidth;															//Height shadowmap


	//Light

	std::vector<Light> lights;													//Vector of light objects
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> structuredBufferSrv;		//View for structured buffer
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> viewBuffers;				//View/proj matrix for each light
	StructuredBuffer<LightStruct>lightBuffer;									//Holds Light data 
	Microsoft::WRL::ComPtr<ID3D11Buffer> numLightBuffer;						//Holds nr of lights
	int LightCap = 10;															//Max nr of lights that can be created

	//Mesh
	
	//std::vector<Mesh> boundingSphere;											//Mesh for visualization

	bool updateViewMatrix(int lightIndex);
	
public:

	LightHandler();
	void addLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 UpDir, int type = 0, float coneAngle = 0.5f);
	bool updateBuffers();
	void setPosition(DirectX::XMFLOAT3 position, int lightIndex);				//changes view matrix
	void setDirection(DirectX::XMFLOAT3 direction, int lightIndex);				//changes view matrix
	void setUpDirection(DirectX::XMFLOAT3 direction, int lightIndex);			//changes view matrix
	void setColor(DirectX::XMFLOAT3 color, int lightIndex);
	void setConeAngle(float angle, int lightIndex);
	void setLightType(float type, int lightIndex);								//0 point, 1 directional, 2 spot
	void setRange(float range, int lightIndex);
	void setFalloff(float falloff, int lightIndex);
	ID3D11Buffer* getViewBuffer(int ltIndex) const;
	int getNrOfLights() const;
	void drawShadows(int lightIndex, std::vector<GameObject> gameObjects);
	void bindLightBuffers(ID3D11Buffer*& cameraPosBuffer, ID3D11ShaderResourceView** dfSrvArr); //Inte klar Behöver skrivas om!
	//void drawDebugMesh();
};