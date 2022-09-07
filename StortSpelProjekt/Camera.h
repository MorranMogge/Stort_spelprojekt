#pragma once
#include <directxmath.h>
#include <d3d11.h>

using namespace DirectX;

class Camera
{
private:
	ID3D11Buffer* cameraBuffer;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
	DirectX::XMMATRIX viewProj;

	DirectX::XMMATRIX rotationMX;
	DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR forwardVec = DEFAULT_FORWARD;
	DirectX::XMVECTOR upVector = DEFAULT_UP;
	DirectX::XMVECTOR rightVec = DEFAULT_RIGHT;
	DirectX::XMVECTOR upVec = DEFAULT_UP;

	DirectX::XMFLOAT4X4 floatMX;
	D3D11_MAPPED_SUBRESOURCE subData = {};

	void updateCamera(ID3D11DeviceContext* immediateContext);

public:
	Camera(ID3D11DeviceContext* immediateContext, ID3D11Device* device);
	~Camera();

	void moveCamera(ID3D11DeviceContext* immediateContext, float dt);
};