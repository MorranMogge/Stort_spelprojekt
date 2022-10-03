#include "stdafx.h"
#include "Light.h"

Light::Light(DirectX::XMFLOAT3 lightColor, DirectX::XMFLOAT3 lightPos, DirectX::XMFLOAT3 FocusPos, DirectX::XMFLOAT3 UpDir, float coneAngle, int type)
	:position(lightPos), color(lightColor), upDirection(UpDir), direction(FocusPos), coneAngle(coneAngle), lightType(type), falloff(1.0), range(200)
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(this->position.x, this->position.y, this->position.z, 0);
	DirectX::XMVECTOR focusPos = DirectX::XMVectorSet(FocusPos.x, FocusPos.y, FocusPos.z, 0);
	DirectX::XMVECTOR upPos = DirectX::XMVectorSet(UpDir.x, UpDir.y, UpDir.z, 0);
	this->view = { DirectX::XMMatrixLookToLH(pos,focusPos, upPos) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 0.75f, 0.1f, 200.0f) };
}

DirectX::XMFLOAT3 Light::getPosition() const
{
	return this->position;
}

DirectX::XMFLOAT3 Light::getColor() const
{
	return this->color;
}

DirectX::XMFLOAT3 Light::getUpDir() const
{
	return this->upDirection;
}

DirectX::XMFLOAT3 Light::getDirection() const
{
	return this->direction;
}

DirectX::XMMATRIX Light::getViewMatrix() const
{
	return this->view;
}

void Light::setPosition(DirectX::XMFLOAT3 position)
{
	this->position = position;
	updateMatrix(position, this->direction, this->upDirection);
}

void Light::setUpDirection(DirectX::XMFLOAT3 direction)
{
	this->upDirection = direction;
	updateMatrix(this->position, this->direction, direction);
}

void Light::setDirection(DirectX::XMFLOAT3 direction)
{
	this->direction = direction;
	updateMatrix(this->position, direction, this->upDirection);
}

void Light::setColor(DirectX::XMFLOAT3 color)
{
	this->color = color;
}

void Light::setConeAngle(float angle)
{
	this->coneAngle = angle;
}

void Light::setLightType(float type)
{
	this->lightType = type;
}

void Light::setRange(float range)
{
	this->range = range;
}

void Light::setFalloff(float falloff)
{
	this->falloff = falloff;
}

float Light::getConeAngle() const
{
	return this->coneAngle;
}

int Light::getType() const
{
	return this->lightType;
}


void Light::updateMatrix(DirectX::XMFLOAT3 lightPos, DirectX::XMFLOAT3 FocusPos, DirectX::XMFLOAT3 UpDir)
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 0);
	DirectX::XMVECTOR focusPos = DirectX::XMVectorSet(FocusPos.x, FocusPos.y, FocusPos.z, 0);
	DirectX::XMVECTOR upPos = DirectX::XMVectorSet(UpDir.x, UpDir.y, UpDir.z, 0);

	this->position = lightPos;
	this->view = { DirectX::XMMatrixLookToLH( pos, focusPos, upPos) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 0.75f, 0.1f, 100.0f) };
}