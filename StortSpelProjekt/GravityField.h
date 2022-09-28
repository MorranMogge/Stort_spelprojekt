#pragma once
#include <SimpleMath.h>

class GravityField
{
private:
	float gravityConstant;
	DirectX::XMFLOAT3 planetCenterPoint;

public:
	GravityField(float gravityConstant = 1.82, DirectX::XMFLOAT3 planetCenterPoint = DirectX::XMFLOAT3(0.0f,0.0f,0.0f));
	~GravityField();
	DirectX::XMFLOAT3 calcGravFactor(DirectX::XMFLOAT3 objectPosition);
	DirectX::XMFLOAT3 calcGravFactor(DirectX::SimpleMath::Vector3 objectPosition);
};

