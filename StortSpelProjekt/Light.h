#pragma once
#include <DirectXMath.h>
#include <d3d11.h>

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

	Light(DirectX::XMFLOAT3 lightColor, DirectX::XMFLOAT3 lightPos, DirectX::XMFLOAT3 FocusPos, DirectX::XMFLOAT3 UpDir, float coneAngle = 0.5f, int type = 0);
	DirectX::XMFLOAT3 getPosition()		const;
	DirectX::XMFLOAT3 getColor()		const;
	DirectX::XMFLOAT3 getUpDir()		const;
	DirectX::XMFLOAT3 getDirection()	const;
	DirectX::XMMATRIX getViewMatrix()	const;
	void setPosition(DirectX::XMFLOAT3 position);		
	void setUpDirection(DirectX::XMFLOAT3 direction);
	void setDirection(DirectX::XMFLOAT3 direction);
	void setColor(DirectX::XMFLOAT3 color);
	void setConeAngle(float angle);
	void setLightType(int type);
	void setRange(float range);
	void setFalloff(float falloff);
	float getConeAngle()	const;
	int getType()			const;
	void updateMatrix(DirectX::XMFLOAT3 lightPos, DirectX::XMFLOAT3 FocusPos, DirectX::XMFLOAT3 UpDir);
};
