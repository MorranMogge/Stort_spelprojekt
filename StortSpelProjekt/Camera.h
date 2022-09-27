#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "ConstantBufferNew.h"
#include "GPU.h"

class Camera
{
private:
	float deltaTime;
	ConstantBufferNew<cameraStruct> cameraBuffer;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;

	DirectX::XMFLOAT3 rotation;
	DirectX::XMVECTOR rotVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	DirectX::XMMATRIX rotationMX;
	DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet(0.0f, 0.0f, -25.0f, 0.0f);
	DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, -20.0f, 0.0f);

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR rightVec = DEFAULT_RIGHT;
	DirectX::XMVECTOR forwardVec = DEFAULT_FORWARD;
	DirectX::XMVECTOR upVector = DEFAULT_UP;

public:
	Camera();
	~Camera();

	void updateCamera();
	void moveCamera(const DirectX::XMVECTOR& playerPosition, const DirectX::XMVECTOR& playerUpVector, const float& deltaTime);
	void AdjustRotation(float x, float y);
};