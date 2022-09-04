#pragma once

#include <SimpleMath.h>

struct OrthoGraphicCamera
{
	DirectX::SimpleMath::Vector3 position{ 0.0f, 2.0f, -10.0f };
	DirectX::SimpleMath::Vector3 rotation{ 0.0f, 0.0f, 0.0f };

	DirectX::SimpleMath::Vector3 lookAt{ 0.0f, 3.5f, 0.0f };
	DirectX::SimpleMath::Vector3 lookTo{ 0.0f, 0.0f, 1.0f };

	DirectX::SimpleMath::Vector3 upVector{ 0.0f, 1.0f, 0.0f };

	float width = 16;
	float height = 9;
	float nearZ = 0.01f;
	float farZ = 100.0f;

	float rotateHorizontal = 0;//rotatin camera around origin, rotate horizontal, similar to rotate around ekvator
	float rotateVertical = 0.1;//rotatin camera around origin, rotate vertical

	bool hasTarget = false;

	DirectX::XMVECTOR TranslateV()
	{
		return 	DirectX::XMVector3Transform(
			position.ToXMVECTOR(),
			//orotate camera position
			DirectX::XMMatrixRotationRollPitchYaw(
				rotateVertical * 0.0174532925f,
				-(rotateHorizontal * 0.0174532925f),
				0.0f));
	}
	DirectX::XMMATRIX RotationM()
	{
		return DirectX::XMMatrixRotationRollPitchYaw(
			rotation.x * 0.0174532925f,
			-(rotation.y * 0.0174532925f),
			rotation.z * 0.0174532925f);
	}
	DirectX::XMMATRIX OrthoGraphicM()
	{
		return TransformMath::OrthographicLH(width, height, nearZ, farZ);
	}
	DirectX::XMMATRIX ViewM()
	{
		if (hasTarget)
		{
			return TransformMath::LookAtLH( //camera transform Matrix is projectionMatrix
				position.ToXMVECTOR(),//position of the camera
				//translate(),//position of the camera
				lookAt.ToXMVECTOR(),//lookAt.toXMVECTOR(), //position to look at, look at origin, zero vector
				upVector.ToXMVECTOR());//up position for the camera, to is pointing up;
		}
		else
		{
			return TransformMath::LookToLH( //camera transform Matrix is projectionMatrix
				position.ToXMVECTOR(),//position of the camera
				//translate(),//position of the camera
				lookTo.ToXMVECTOR(),//lookAt.toXMVECTOR(), //position to look at, look at origin, zero vector
				upVector.ToXMVECTOR());//up position for the camera, to is pointing up;
		}
	}
	Matrix4X4 ViewOrthoM()
	{
		Matrix4X4 cameraMatrix;
		DirectX::XMStoreFloat4x4(&cameraMatrix, ViewM() * RotationM() * OrthoGraphicM());
		cameraMatrix.Transpose();
		return cameraMatrix;
	}
};