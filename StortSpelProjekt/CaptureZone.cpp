#include "stdafx.h"
#include "CaptureZone.h"

CaptureZone::CaptureZone(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 rotation, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 color)
{
	tempMesh = new Mesh("../Meshes/zone");
	capMesh = new GameObject(tempMesh, position, rotation, ObjID::MOVABLE, nullptr, scale);
	
	//Set up color buffer
	this->colorBuffer.Initialize(GPU::device, GPU::immediateContext);
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
}

CaptureZone::~CaptureZone()
{
	delete this->capMesh;
	delete this->tempMesh;
}

void CaptureZone::setColor(const DirectX::SimpleMath::Vector3 & color)
{
	//Set up color buffer
	this->colorBuffer.getData() = DirectX::XMFLOAT4(color.x, color.y, color.z, 0.1f);
	this->colorBuffer.applyData();
}

void CaptureZone::setPosition(const DirectX::SimpleMath::Vector3& position)
{
	this->capMesh->setPos(position);
}

void CaptureZone::setRotation(const DirectX::SimpleMath::Vector3& rotation)
{
	this->capMesh->setRot(rotation);
}

void CaptureZone::setScale(const DirectX::SimpleMath::Vector3& scale)
{
	this->capMesh->setScale(scale);
}

void CaptureZone::draw()
{
	GPU::immediateContext->PSSetConstantBuffers(2, 1, this->colorBuffer.getReferenceOf());
	this->capMesh->draw();
}
