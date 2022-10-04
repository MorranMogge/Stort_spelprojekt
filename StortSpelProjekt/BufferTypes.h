#pragma once
#include "stdafx.h"

struct MyStruct
{
	float testf;
};

struct cameraStruct
{
	DirectX::XMMATRIX viewProjMX;
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

struct particleStruct
{
	DirectX::XMFLOAT3 pos;
	float delta;
	float lifeTime;
	particleStruct(const DirectX::XMFLOAT3 & position, const float deltaTime, const float lifeTime)
	{
		this->pos = position;
		this->delta = deltaTime;
		this->lifeTime = lifeTime;
	}
};