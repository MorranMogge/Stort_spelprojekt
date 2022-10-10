#include "Light.h"
#include "GPU.h"

Light::Light(const DirectX::XMFLOAT3 &lightColor, const DirectX::XMFLOAT3 &lightPos, const DirectX::XMFLOAT3 &FocusPos, const DirectX::XMFLOAT3 &UpDir, float coneAngle, int type, float range, float falloff)
	:position(lightPos), color(lightColor), upDirection(UpDir), direction(FocusPos), coneAngle(coneAngle), lightType(type), falloff(falloff), range(range)
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(this->position.x, this->position.y, this->position.z, 0);
	DirectX::XMVECTOR focusPos = DirectX::XMVectorSet(FocusPos.x, FocusPos.y, FocusPos.z, 0);
	DirectX::XMVECTOR upPos = DirectX::XMVectorSet(UpDir.x, UpDir.y, UpDir.z, 0);

	static const DirectX::XMMATRIX t(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	this->view = { DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(pos,focusPos, upPos) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 1.0f /*(float)GPU::windowWidth/ (float)GPU::windowHeight*/, 0.1f, 200.0f)) };
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

void Light::setPosition(const DirectX::XMFLOAT3 &position)
{
	this->position = position;
	updateMatrix(position, this->direction, this->upDirection);
}

void Light::setUpDirection(const DirectX::XMFLOAT3 &direction)
{
	this->upDirection = direction;
	updateMatrix(this->position, this->direction, direction);
}

void Light::setDirection(const DirectX::XMFLOAT3 &direction)
{
	this->direction = direction;
	updateMatrix(this->position, direction, this->upDirection);
}

void Light::setColor(const DirectX::XMFLOAT3 &color)
{
	this->color = color;
}

void Light::setConeAngle(const float &angle)
{
	this->coneAngle = angle;
}

void Light::setLightType(const int &type)
{
	this->lightType = type;
}

void Light::setRange(const float &range)
{
	this->range = range;
}

void Light::setFalloff(const float &falloff)
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

float Light::getFalloff() const
{
	return this->falloff;
}

float Light::getRange() const
{
	return this->range;
}


void Light::updateMatrix(const DirectX::XMFLOAT3 &lightPos, const DirectX::XMFLOAT3 &FocusPos, const DirectX::XMFLOAT3 &UpDir)
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 0);
	DirectX::XMVECTOR focusPos = DirectX::XMVectorSet(FocusPos.x, FocusPos.y, FocusPos.z, 0);
	DirectX::XMVECTOR upPos = DirectX::XMVectorSet(UpDir.x, UpDir.y, UpDir.z, 0);

	static const DirectX::XMMATRIX t(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	this->position = lightPos;
	this->view = { DirectX::XMMatrixTranspose( DirectX::XMMatrixLookToLH( pos, focusPos, upPos) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PI / 2.0f, 1.0f, 0.1f, 100.0f) ) };
}