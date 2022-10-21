#include "stdafx.h"
#include "Camera.h"
#include <string>
using namespace DirectX;

void Camera::updateCamera()
{
	viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	XMStoreFloat3(&position, this->cameraPos);
	positionBuffer.getData().pos = position;
	positionBuffer.getData().padding = 0;
	this->upVectorBuffer.getData().pos = DirectX::SimpleMath::Vector3(upVector);
	this->upVectorBuffer.getData().padding = 0;

	this->upVectorBuffer.applyData();
	this->positionBuffer.applyData();
	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::Camera()
{
	this->deltaTime = 0.0f;
	this->position = { 0.0f, 0.0f, 0.0f };

	cameraBuffer.Initialize(GPU::device, GPU::immediateContext);
	positionBuffer.Initialize(GPU::device, GPU::immediateContext);
	upVectorBuffer.Initialize(GPU::device, GPU::immediateContext);

	XMFLOAT3 position(0, 0, 0);
	XMStoreFloat3(&position, this->cameraPos);
	positionBuffer.getData().pos = position;
	positionBuffer.getData().padding = 0;
	
	this->upVectorBuffer.getData().pos = DirectX::SimpleMath::Vector3(upVector);
	this->upVectorBuffer.getData().padding = 0;
		
	rotationMX = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1264.f / 681.f, 0.1f, 800.0f);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	this->upVectorBuffer.applyData();
	this->positionBuffer.applyData();
	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::~Camera()
{
}

void Camera::moveCamera(const DirectX::XMVECTOR& playerPosition, const DirectX::XMMATRIX& playerRotation, const float& deltaTime)
{
	this->deltaTime = deltaTime;

	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, playerRotation * DirectX::XM_PI);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, playerRotation * DirectX::XM_PI);
	upVector = XMVector3TransformCoord(DEFAULT_UP, playerRotation * DirectX::XM_PI);

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

void Camera::VSbindPositionBuffer(const int& slot)
{
	GPU::immediateContext->VSSetConstantBuffers(slot, 1, this->positionBuffer.getReferenceOf());
}

void Camera::VSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->VSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}

void Camera::PSbindPositionBuffer(const int& slot)
{
	GPU::immediateContext->PSSetConstantBuffers(slot, 1, this->positionBuffer.getReferenceOf());
}

void Camera::PSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->PSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}

void Camera::GSbindPositionBuffer(const int& slot)
{
	GPU::immediateContext->GSSetConstantBuffers(slot, 1, this->positionBuffer.getReferenceOf());
}

void Camera::GSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->GSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}

void Camera::GSbindUpBuffer(const int& slot)
{
	GPU::immediateContext->GSSetConstantBuffers(slot, 1, this->upVectorBuffer.getReferenceOf());
}

void Camera::CSbindUpBuffer(const int& slot)
{
	GPU::immediateContext->CSSetConstantBuffers(slot, 1, this->upVectorBuffer.getReferenceOf());
}