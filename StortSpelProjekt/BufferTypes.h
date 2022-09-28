#pragma once
#include <d3d11.h>

struct posStruct
{
	DirectX::XMFLOAT3 pos;
	float padding;
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
	LightStruct(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4 direction, float coneAngle, float type, float range, float falloff, DirectX::XMFLOAT4X4 matrix)
	{
		this->pos = position;
		this->color = color;
		this->viewMatrix = matrix;
		this->direction = direction;
		this->angleTypePadding.x = coneAngle;
		this->angleTypePadding.y = type;
		this->angleTypePadding.z = range;
		this->angleTypePadding.w = falloff;
	}
};

struct particleStruct
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 startpos;
	float delta;
	float lifeTime;
	particleStruct(const DirectX::XMFLOAT3 & position, const float deltaTime, const float lifeTime)
	{
		this->startpos = position;
		this->pos = position;
		this->delta = deltaTime;
		this->lifeTime = lifeTime;
	}
};