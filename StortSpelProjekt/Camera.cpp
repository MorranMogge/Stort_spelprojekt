#include "Camera.h"
#include <string>
using namespace DirectX;

void Camera::updateCamera()
{
	viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::Camera()
{
	cameraBuffer.Initialize(GPU::device, GPU::immediateContext);

	rotationMX = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1264.f / 681.f, 0.1f, 800.0f);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::~Camera()
{
}

void Camera::moveCamera(const DirectX::XMVECTOR& playerPosition, const DirectX::XMMATRIX& playerRotation, const float& deltaTime)
{
	this->deltaTime = deltaTime;

	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, playerRotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, playerRotation);
	upVector = XMVector3TransformCoord(DEFAULT_UP, playerRotation);

	cameraPos = playerPosition - forwardVector * 10.0f + upVector * 40.0f;
	lookAtPos = playerPosition;
	updateCamera();
}

DirectX::XMVECTOR Camera::getForwardVector() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Camera::getRightVector() const
{
	return this->rightVector;
}
