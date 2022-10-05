#pragma once
#include "stdafx.h"

class Light
{
private:
	DirectX::XMMATRIX view;
	DirectX::XMFLOAT3 upDirection;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;
	float coneAngle;
	int lightType;	//0 point, 1 directional, 2 spot
	float range;
	float falloff;

public:

	Light(const DirectX::XMFLOAT3 &lightColor, const DirectX::XMFLOAT3 &lightPos, const DirectX::XMFLOAT3 &FocusPos, const DirectX::XMFLOAT3 &UpDir, float coneAngle = 0.5f, int type = 0, float range = 50, float falloff = 1);
	DirectX::XMFLOAT3 getPosition()		const;
	DirectX::XMFLOAT3 getColor()		const;
	DirectX::XMFLOAT3 getUpDir()		const;
	DirectX::XMFLOAT3 getDirection()	const;
	DirectX::XMMATRIX getViewMatrix()	const;
	void setPosition(const DirectX::XMFLOAT3 &position);
	void setUpDirection(const DirectX::XMFLOAT3 &direction);
	void setDirection(const DirectX::XMFLOAT3 &direction);
	void setColor(const DirectX::XMFLOAT3 &color);
	void setConeAngle(const float &angle);
	void setLightType(const int &type);
	void setRange(const float &range);
	void setFalloff(const float &falloff);
	float getConeAngle()	const;
	int getType()			const;
	float getFalloff()		const;
	float getRange()		const;
	void updateMatrix(const DirectX::XMFLOAT3 &lightPos, const DirectX::XMFLOAT3 &FocusPos, const DirectX::XMFLOAT3 &UpDir);
};
