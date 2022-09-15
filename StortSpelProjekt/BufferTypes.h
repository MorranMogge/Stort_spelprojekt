#pragma once
#include <d3d11.h>

struct MyStruct
{
	float testf;
};

struct LightStruct
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 angleTypePadding;
	DirectX::XMFLOAT4X4 viewMatrix;
	LightStruct(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4 direction, float coneAngle, float type, DirectX::XMFLOAT4X4 matrix)
	{
		this->pos = position;
		this->color = color;
		this->viewMatrix = matrix;
		this->direction = direction;
		this->angleTypePadding.x = coneAngle;
		this->angleTypePadding.y = type;
		this->angleTypePadding.z = 1;
		this->angleTypePadding.w = 1;
	}
};