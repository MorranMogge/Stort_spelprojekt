#pragma once
#include <directxmath.h>
#include <d3d11.h>

using namespace DirectX; //Promise to remove this - Klara

class Camera
{
private:
	ID3D11Buffer* cameraBuffer; // Is realesed
	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;
	XMMATRIX viewProj;

	XMMATRIX rotationMX;
	XMVECTOR cameraPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR lookAtPos = XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);

	const XMVECTOR DEFAULT_RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMVECTOR forwardVec = DEFAULT_FORWARD;
	XMVECTOR upVector = DEFAULT_UP;
	XMVECTOR rightVec = DEFAULT_RIGHT;
	XMVECTOR upVec = DEFAULT_UP;

	XMFLOAT4X4 floatMX;
	D3D11_MAPPED_SUBRESOURCE subData = {};

	void updateCamera(ID3D11DeviceContext* immediateContext);

public:
	Camera(ID3D11DeviceContext* immediateContext, ID3D11Device* device);
	~Camera();

	void moveCamera(ID3D11DeviceContext* immediateContext, float dt);
};