#include "Camera.h"
using namespace DirectX;
#include <string>

void Camera::updateCamera()
{
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::Camera()
{
	cameraBuffer.Initialize(GPU::device, GPU::immediateContext);

	rotationMX = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1264.f / 681.f, 0.1f, 800.0f);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::~Camera()
{
}

void Camera::moveCamera(const DirectX::XMVECTOR& playerPosition, const DirectX::XMVECTOR& playerUpVector, const float& deltaTime)
{
	this->deltaTime = deltaTime;

	//rotationMX = XMMatrixRotationRollPitchYawFromVector(playerRotation);
	//rotationMX = XMMatrixRotationX(playerRotation.x);
	//rotationMX = XMMatrixRotationRollPitchYaw(playerRotation.x, 0.0f, 0.0f);

	//upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);
	//rightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
	forwardVec = playerUpVector;

	cameraPos = playerPosition - forwardVec * 15 + upVector * 1;
	lookAtPos = playerPosition;
	updateCamera();
}

void Camera::AdjustRotation(float x, float y)
{
	rotation.x += x;
	rotation.y += y;
	rotVector = XMLoadFloat3(&rotation);

	rotationMX = XMMatrixRotationRollPitchYawFromVector(rotVector);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);
	rightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
	forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);

	updateCamera();
}