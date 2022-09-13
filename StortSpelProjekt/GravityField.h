#pragma once
#include <DirectXMath.h>

class GravityField
{
private:
	float gravityConstant;
	DirectX::XMFLOAT3 planetCenterPoint;

public:
	GravityField(float gravityConstant = 0.0982, DirectX::XMFLOAT3 planetCenterPoint = DirectX::XMFLOAT3(0.0f,0.0f,0.0f));
	~GravityField();
	DirectX::XMFLOAT3 calcGravFactor(DirectX::XMFLOAT3 objectPosition);
};

