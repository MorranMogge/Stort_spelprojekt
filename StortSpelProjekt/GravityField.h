#pragma once
#include "stdafx.h"

class GravityField
{
private:
	float gravityConstant;
	float radius;
	DirectX::XMFLOAT3 planetCenterPoint;

public:
	GravityField(float gravityConstant = 0.82, DirectX::XMFLOAT3 planetCenterPoint = DirectX::XMFLOAT3(0.0f,0.0f,0.0f), float radius = 1.0f);
	~GravityField();
	DirectX::XMFLOAT3 calcGravFactor(DirectX::XMFLOAT3 objectPosition);
	DirectX::XMFLOAT3 calcGravFactor(DirectX::SimpleMath::Vector3 objectPosition);
};

