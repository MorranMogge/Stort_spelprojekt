#pragma once
#include "Light.h"
#include <wrl.h>
#include "ConstantBufferNew.h"
#include "StructuredBuffer.h"
#include "GPU.h"
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"


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
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> viewBuffers;				//View/proj matrix for each light
	StructuredBuffer<LightStruct>lightBuffer;									//Holds Light data 
	Microsoft::WRL::ComPtr<ID3D11Buffer> numLightBuffer;						//Holds nr of lights
	int LightCap = 10;															//Max nr of lights that can be created

	//Debug Mesh
	Mesh* debugMesh;
	std::vector<GameObject*> boundingSphere;									//Mesh for visualization
	
public:

	LightHandler();
	~LightHandler();
	void addLight(const DirectX::XMFLOAT3 &position, const DirectX::XMFLOAT3 &color, const DirectX::XMFLOAT3 &direction, const DirectX::XMFLOAT3 &UpDir, int type = 0, float coneAngle = 0.5f, float range = 50, float falloff = 1);
	bool updateBuffers();
	void setPosition(const DirectX::XMFLOAT3 &position, const int& lightIndex);				//changes view matrix
	void setDirection(const DirectX::XMFLOAT3 &direction, const int& lightIndex);			//changes view matrix
	void setUpDirection(const DirectX::XMFLOAT3 &direction, const int& lightIndex);			//changes view matrix
	void setColor(const DirectX::XMFLOAT3 &color, const int& lightIndex);
	void setConeAngle(const float &angle, const int &lightIndex);
	void setLightType(const int& type, const int& lightIndex);								//0 point, 1 directional, 2 spot
	void setRange(const float &range, const int& lightIndex);
	void setFalloff(const float &falloff, const int& lightIndex);
	ID3D11Buffer* getViewBuffer(const int& lightIndex) const;
	int getNrOfLights() const;
	void drawShadows(const int& lightIndex, const std::vector<Player*>& gameObjects, Camera* stageCamera = nullptr);
	void drawShadows(const int &lightIndex, const std::vector<GameObject*> &gameObjects, Camera* stageCamera = nullptr);
	void bindLightBuffers();
	void drawDebugMesh();
	void unbindSrv();

private:

	bool updateViewMatrix(const int &lightIndex);
};