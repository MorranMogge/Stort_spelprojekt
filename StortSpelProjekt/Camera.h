#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "ConstantBufferNew.h"
#include "GPU.h"
#include "Planet.h"

class Camera
{
private:
	float fieldOfView = 0.76f;
	float maxFOV = 0.8f;
	float minFOV = 0.6f;

	DirectX::XMFLOAT3 position;
	DirectX::SimpleMath::Vector3 resultVector;
	ConstantBufferNew<cameraStruct> cameraBuffer;
	ConstantBufferNew<posStruct> positionBuffer;
	ConstantBufferNew<posStruct> upVectorBuffer;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projMatrix;
	DirectX::XMMATRIX rotationMX;
	DirectX::XMVECTOR logicalPos = DirectX::XMVectorSet(0.0f, 60.0f, -30.0f, 0.0f);
	DirectX::XMVECTOR cameraPos = DirectX::XMVectorSet(0.0f, 60.0f, -30.0f, 0.0f);
	DirectX::XMVECTOR oldCameraPos = cameraPos;
	DirectX::XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 42.0f, 0.0f, 0.0f);

	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	DirectX::XMVECTOR rightVector = DEFAULT_RIGHT;
	DirectX::XMVECTOR forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR logicalUp = DEFAULT_UP;
	DirectX::XMVECTOR upVector = logicalUp;


	DirectX::XMVECTOR velocityVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

public:
	Camera();
	~Camera();

	void updateCamera();
	void moveCamera(const DirectX::XMVECTOR& playerPosition, const DirectX::XMMATRIX& playerRotation, const DirectX::XMVECTOR& playerUp, const float& playerSpeed, const float& deltaTime);
	void moveVelocity(const DirectX::XMVECTOR& playerPosition, const DirectX::XMMATRIX& playerRotation, const DirectX::XMVECTOR& playerUp, const float& playerSpeed, const float& deltaTime);

	void winScene(const DirectX::XMVECTOR& shipPosition, const DirectX::XMMATRIX& shipRotation);
	void landingMinigameScene(const Planet* planet, const DirectX::XMVECTOR& shipPosition, const DirectX::XMMATRIX& shipRotation);
	DirectX::XMVECTOR getForwardVector() const;
	DirectX::XMVECTOR getRightVector() const;
	DirectX::XMVECTOR getUpVector() const;
	DirectX::XMVECTOR getPosition() const;
	ID3D11Buffer* getViewBuffer();
	ID3D11Buffer* getPositionBuffer();

	void VSbindPositionBuffer(const int &slot);
	void VSbindViewBuffer(const int& slot);
	void PSbindPositionBuffer(const int& slot);
	void PSbindViewBuffer(const int& slot);
	void GSbindPositionBuffer(const int& slot);
	void GSbindViewBuffer(const int& slot);
	void GSbindUpBuffer(const int& slot);
	void CSbindUpBuffer(const int& slot);
};