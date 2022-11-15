#include "stdafx.h"
#include "CaptureZone.h"


CaptureZone::CaptureZone(Mesh* useMesh, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 rotation, GravityField* field, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 color)
	:GameObject(useMesh, position, rotation, ObjID::MOVABLE, field, scale)
{	
	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
}

void CaptureZone::setColor(const DirectX::SimpleMath::Vector3 & color)
{
	//Set up color buffer
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
}

bool CaptureZone::detectedObject(GameObject* objectToCheck)
{
	bool didDetect = false;
	
	if (this->withinRadious(objectToCheck, this->scale.x))
	{
		didDetect = true;
	}
	else
	{
		this->setScale(DirectX::XMFLOAT3(2, 2, 2));
	}
	return didDetect;
}

void CaptureZone::drawFresnel()
{
	GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
	this->draw();
}
