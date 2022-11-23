#pragma once
#include "stdafx.h"

class GravityField
{
private:
	float gravityConstant;
	float radius;
	DirectX::XMFLOAT3 planetCenterPoint;

public:
	GravityField(const float& gravityConstant = 109.82, const DirectX::XMFLOAT3& planetCenterPoint = DirectX::XMFLOAT3(0.0f,0.0f,0.0f), const float& radius = 1.0f);
	~GravityField();	
	void setCenterpoint(const DirectX::XMFLOAT3& newPos);
	DirectX::XMFLOAT3 calcGravFactor(DirectX::XMFLOAT3 objectPosition);
	DirectX::XMFLOAT3 calcGravFactor(const DirectX::SimpleMath::Vector3& objectPosition);
};