#pragma once

#include <SimpleMath.h>

struct PerspectiveCamera
{
	DirectX::SimpleMath::Vector3 position{ 0.0f, 5.0f, -20.0f };
	DirectX::SimpleMath::Vector3 rotation{ 0.0f, 0.0f, 0.0f };

	DirectX::SimpleMath::Vector3 lookAt{ 0.0f, 0.0f, 0.0f };
	DirectX::SimpleMath::Vector3 lookTo{ 0.0f, 0.0f, 1.0f };

	DirectX::SimpleMath::Vector3 upVector{ 0.0f, 1.0f, 0.0f };

	float aspectRatio = 1.0f;

	float fovAngle = 45.0f * 0.0174532925f; //3.14159265359f / 4.0f;  
	float nearZ = 0.01f;
	float farZ = 1000.0f;

	bool hasTarget = false;

	//float rotateHorizontal = 0.0f;//rotatin camera around origin, rotate horizontal, similar to rotate around ekvator
	//float rotateVertical = 0.0f;//rotatin camera around origin, rotate vertical

	//DirectX::XMVECTOR TranslateV() const
	//{
	//	return 	DirectX::XMVector3Transform(
	//		DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f),
	//		DirectX::XMMatrixRotationRollPitchYaw(
	//			rotateVertical * 0.0174532925f,
	//			-(rotateHorizontal * 0.0174532925f),
	//			0.0f));
	//}
	//DirectX::XMMATRIX RotationM() const
	//{
	//	return DirectX::XMMatrixRotationRollPitchYaw(
	//		rotation.x * 0.0174532925f,
	//		-(rotation.y * 0.0174532925f),
	//		rotation.z * 0.0174532925f);
	//}
	//DirectX::XMMATRIX ProjectionM() const
	//{
	//	return DirectX::XMMatrixPerspectiveLH(fovAngle, aspectRatio, nearZ, farZ);
	//}
	//DirectX::XMMATRIX ViewM() const
	//{
	//	if (hasTarget)
	//	{
	//		return DirectX::XMMatrixLookAtLH( //camera transform Matrix is projectionMatrix
	//			DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f),//position of the camera
	//			//translate(),//position of the camera
	//			DirectX::XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 0.0f),//lookAt.toXMVECTOR(), //position to look at, look at origin, zero vector
	//			DirectX::XMVectorSet(upVector.x, upVector.y, upVector.z, 0.0f));//up position for the camera, to is pointing up;
	//	}
	//	else
	//	{
	//		return DirectX::XMMatrixLookToLH( //camera transform Matrix is projectionMatrix
	//			//position.toXMVECTOR0(),//position of the camera
	//			TranslateV(),//position of the camera
	//			//DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f),//position of the camera
	//			DirectX::XMVectorSet(lookTo.x, lookTo.y, lookTo.z, 0.0f),//lookAt.toXMVECTOR(), //position to look at, look at origin, zero vector
	//			DirectX::XMVectorSet(upVector.x, upVector.y, upVector.z, 0.0f));//up position for the camera, to is pointing up;
	//	}
	//}
	DirectX::XMMATRIX ViewProjectionM() const
	{
		using namespace DirectX;
		XMVECTOR cameraPos = XMVectorSet(position.x, position.y, position.z, 0);
		XMVECTOR upDir = XMVectorSet(upVector.x, upVector.y, upVector.z, 0);
		
		XMMATRIX view;
		if (hasTarget)
		{
			XMVECTOR focusPos = XMVectorSet(lookAt.x, lookAt.y, lookAt.z, 0);
			view = { (XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z)) * XMMatrixLookAtLH(cameraPos,focusPos, upDir) *
				XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearZ, farZ) };
		}
		else
		{
			XMVECTOR focusPos = XMVectorSet(lookTo.x, lookTo.y, lookTo.z, 0);
			view = { (XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z)) * XMMatrixLookToLH(cameraPos,focusPos, upDir) *
				XMMatrixPerspectiveFovLH(fovAngle, aspectRatio, nearZ, farZ) };
		}
		return XMMatrixTranspose(view);

		//DirectX::XMMATRIX cameraMatrix = ViewM() * RotationM() * ProjectionM();
		//return DirectX::XMMatrixTranspose(cameraMatrix);
	}
};