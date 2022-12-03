#include "stdafx.h"
#include "CaptureZone.h"


CaptureZone::CaptureZone(Mesh* useMesh, const DirectX::SimpleMath::Vector3 & position, const DirectX::SimpleMath::Vector3 & rotation, GravityField* field, const DirectX::SimpleMath::Vector3 & scale, const DirectX::SimpleMath::Vector3 & color)
	:GameObject(useMesh, position, rotation, ObjID::MOVABLE, field, scale), radius(scale.x)
{	
	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
	this->setRot(getRotOrientedToGrav());
	this->position = position;
}

void CaptureZone::setColor(const DirectX::SimpleMath::Vector3 & color)
{
	//Set up color buffer
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
}

bool CaptureZone::detectedObject(GameObject* objectToCheck) const
{
	bool didDetect = false;
	
	if (this->withinRadious(objectToCheck, this->scale.x))
	{
		didDetect = true;
	}
	return didDetect;
}

void CaptureZone::drawFresnel()
{
	GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
	this->draw();
}

float CaptureZone::getRadius() const
{
	return this->radius;
}

void CaptureZone::setRadius(const float& radius)
{
	this->radius = radius;
	this->setScale(DirectX::XMFLOAT3(radius, radius, radius));
}

DirectX::SimpleMath::Vector3 CaptureZone::getPosition()const
{
	return this->position;
}

