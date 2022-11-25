#pragma once
#include "stdafx.h"

enum PlanetShape
{
	NONDISCLOSEDSHAPE,
	SPHERE,
	BOX
};

class GravityField
{
private:
	float gravityConstant;
	float radius;
	DirectX::SimpleMath::Vector3 planetCenterPoint;
	int shape;
	DirectX::SimpleMath::Vector3 cubeScale;

public:
	GravityField(const float& gravityConstant = 109.82, const DirectX::XMFLOAT3& planetCenterPoint = DirectX::XMFLOAT3(0.0f,0.0f,0.0f), const float& radius = 1.0f);
	~GravityField();	
	void setCenterpoint(const DirectX::XMFLOAT3& newPos);
	void setShape(const PlanetShape& shape = PlanetShape::SPHERE, const DirectX::SimpleMath::Vector3& cubeScale = DirectX::SimpleMath::Vector3(1.0f,1.0f,1.0f));
	DirectX::XMFLOAT3 calcGravFactor(DirectX::XMFLOAT3 objectPosition);
	DirectX::XMFLOAT3 calcGravFactor(const DirectX::SimpleMath::Vector3& objectPosition);
};