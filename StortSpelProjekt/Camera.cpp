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

void Camera::moveCamera(const DirectX::XMVECTOR& playerPosition, const DirectX::SimpleMath::Vector3& playerRotation, const DirectX::XMVECTOR& playerUp, const DirectX::XMVECTOR& playerForward, const DirectX::XMVECTOR& playerRight, const float& deltaTime)
{
	this->deltaTime = deltaTime;

	//rotationMX = XMMatrixRotationRollPitchYawFromVector(playerRotation);
	//rotationMX = XMMatrixRotationRollPitchYaw(playerRotation.x + XM_PI/2, playerRotation.y, 0.0f);
	/*rotationMX = XMMatrixRotationRollPitchYaw(playerRotation.x + XM_PIDIV2, playerRotation.z, 0.0f);

	rightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
	forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);*/

	cameraPos = playerPosition - forwardVec * 55;
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

DirectX::XMVECTOR Camera::getRightVector()
{
	return this->rightVec;
}