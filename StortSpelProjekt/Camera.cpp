#include "Camera.h"
using namespace DirectX;
#include <string>

void Camera::updateCamera(ID3D11DeviceContext* immediateContext)
{
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	viewProj = viewMatrix * projMatrix;
	viewProj = XMMatrixTranspose(viewProj);

	XMStoreFloat4x4(&floatMX, viewProj);
	ZeroMemory(&subData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
	memcpy(subData.pData, &floatMX, sizeof(floatMX));
	immediateContext->Unmap(cameraBuffer, 0);
	immediateContext->VSSetConstantBuffers(1, 1, &cameraBuffer);
}

Camera::Camera(ID3D11DeviceContext* immediateContext, ID3D11Device* device)
{
	rotationMX = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1264.f / 681.f, 0.1f, 800.0f);
	viewProj = viewMatrix * projMatrix;
	viewProj = XMMatrixTranspose(viewProj);

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(viewProj);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, 0, &cameraBuffer);
	immediateContext->VSSetConstantBuffers(1, 1, &cameraBuffer);
}

Camera::~Camera()
{
	cameraBuffer->Release();
}

void Camera::moveCamera(ID3D11DeviceContext* immediateContext, const DirectX::XMVECTOR& playerPosition, const float& deltaTime)
{
	this->deltaTime = deltaTime;

	cameraPos = (playerPosition - forwardVec * 10 + upVector * 30);
	lookAtPos = (playerPosition);
	updateCamera(immediateContext);
}

void Camera::AdjustRotation(ID3D11DeviceContext* immediateContext, float x, float y)
{
	rotation.x += x;
	rotation.y += y;
	rotVector = XMLoadFloat3(&rotation);

	rotationMX = XMMatrixRotationRollPitchYawFromVector(rotVector);
	upVector = XMVector3TransformCoord(DEFAULT_UP, rotationMX);
	rightVec = XMVector3TransformCoord(DEFAULT_RIGHT, rotationMX);
	forwardVec = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX);
	lookAtPos = XMVector3TransformCoord(DEFAULT_FORWARD, rotationMX) + cameraPos;

	updateCamera(immediateContext);
}

const DirectX::XMVECTOR Camera::getForwardVec()
{
	return this->forwardVec * deltaTime * 40;
}

const DirectX::XMVECTOR Camera::getRightVec()
{
	return this->rightVec * deltaTime * 40;
}