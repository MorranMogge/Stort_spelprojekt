#include "Camera.h"
using namespace DirectX;

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
	viewMatrix = XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.8f, 1080 / 720, 0.1f, 800.0f);
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

void Camera::moveCamera(ID3D11DeviceContext* immediateContext, float dt)
{
	if (GetAsyncKeyState((VK_SHIFT)))
	{
		dt *= 5.0f;
	}

	if (GetAsyncKeyState('W'))
	{
		forwardVec = DEFAULT_FORWARD;
		cameraPos += forwardVec * 10 * dt;
		lookAtPos += forwardVec * 10 * dt;
		updateCamera(immediateContext);
	}

	else if (GetAsyncKeyState('S'))
	{
		forwardVec = DEFAULT_FORWARD;
		cameraPos -= forwardVec * 10 * dt;
		lookAtPos -= forwardVec * 10 * dt;
		updateCamera(immediateContext);
	}

	if (GetAsyncKeyState('D'))
	{
		rightVec = DEFAULT_RIGHT;
		cameraPos += rightVec * 10 * dt;
		lookAtPos += rightVec * 10 * dt;
		updateCamera(immediateContext);
	}

	else if (GetAsyncKeyState('A'))
	{
		rightVec = DEFAULT_RIGHT;
		cameraPos -= rightVec * 10 * dt;
		lookAtPos -= rightVec * 10 * dt;
		updateCamera(immediateContext);
	}
}