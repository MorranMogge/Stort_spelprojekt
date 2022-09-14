#pragma once
#include <directxmath.h>
#include <d3d11.h>

class Camera
{
private:
	ID3D11Buffer* cameraBuffer;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
	DirectX::XMMATRIX viewProj;

	DirectX::XMFLOAT3 rotation;
	DirectX::XMVECTOR rotVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX rotationMX;
	DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet(0.0f, 0.0f, -45.0f, 0.0f);
	DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, -20.0f, 0.0f);

	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 15.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 55.0f, 0.0f);

	DirectX::XMVECTOR forwardVec = DEFAULT_FORWARD;
	DirectX::XMVECTOR upVector = DEFAULT_UP;

	DirectX::XMFLOAT4X4 floatMX;
	D3D11_MAPPED_SUBRESOURCE subData = {};

public:
	Camera(ID3D11DeviceContext* immediateContext, ID3D11Device* device);
	~Camera();

	void updateCamera(ID3D11DeviceContext* immediateContext);
	void moveCamera(ID3D11DeviceContext* immediateContext);
	void AdjustRotation(ID3D11DeviceContext* immediateContext, float x, float y);
};