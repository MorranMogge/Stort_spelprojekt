#include "stdafx.h"
#include "Arrow.h"
#include "Input.h"
using namespace DirectX;

Arrow::Arrow(const std::string& objectPath, const DirectX::XMFLOAT3& pos)
{
	OBJ testObj(objectPath);
	this->mesh = new Mesh(testObj);

	// load all materials for Obj
	int nrOfMat = (int)testObj.mtl.materials.size();
	for (int i = 0; i < nrOfMat; i++)
	{
		MaterialLibrary::LoadMaterial(testObj.mtl.materials[i]);
	}

	// set position
	this->position = pos;
	this->mesh->position = pos;

	// set rotation
	this->rotation = DirectX::XMMatrixIdentity();
	this->mesh->rotation = this->rotation;
	scale = DirectX::XMFLOAT3(1, 1, 1);

	// set scale
	this->mesh->scale = scale;
	this->scale = scale;

	this->mesh->UpdateCB();
}

Arrow::~Arrow()
{
	if (this->mesh != nullptr)
	{
		delete this->mesh;
	}
}

DirectX::XMVECTOR Arrow::getUpVectorArrow() const
{
	return this->normalVector;
}

DirectX::XMVECTOR Arrow::getForwardVectorArrow() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Arrow::getRightVectorArrow() const
{
	return this->rightVector;
}

DirectX::XMVECTOR Arrow::getArrowVectorArrow() const
{
	return this->arrowVector;
}

DirectX::XMVECTOR Arrow::getPosition() const
{
	return this->position;
}

void Arrow::moveWithCamera(const DirectX::XMVECTOR& cameraPosition, const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraUp, const DirectX::XMMATRIX& cameraRotation)
{
	this->position = cameraPosition - cameraUp * 40.f + cameraForward * 40.f;
	this->rotation = cameraRotation;
}

void Arrow::showDirection(const DirectX::XMVECTOR& goalPosition, const DirectX::XMVECTOR& playerPosition, const DirectX::XMFLOAT3& gravity)
{
	//Calculations
	arrowVector = XMVectorSubtract(goalPosition, playerPosition);
	normalVector = DirectX::XMVectorSet(-gravity.x, -gravity.y, -gravity.z, 1.0f);
	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	normalVector = DirectX::XMVector3Normalize(normalVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);
	
	arrowVector = DirectX::XMVector3Normalize(arrowVector);

	//Rotating
	resultVector = DirectX::XMVector3Dot(arrowVector, rightVector);
	if (resultVector.x < 0.f)
	{
		resultVector = DirectX::XMVector3AngleBetweenNormals(arrowVector, forwardVector);
		rotation *= DirectX::XMMatrixRotationAxis(normalVector, -resultVector.x);
	}
	else if (resultVector.x > 0.f)
	{
		resultVector = DirectX::XMVector3AngleBetweenNormals(arrowVector, forwardVector);
		rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x);
	}
	else
	{
		resultVector = DirectX::XMVector3AngleBetweenNormals(arrowVector, forwardVector);
		if (resultVector.x > XM_PIDIV2) rotation *= DirectX::XMMatrixRotationAxis(normalVector, resultVector.x);
	}

	rightVector = DirectX::XMVector3TransformCoord(DEFAULT_RIGHT, rotation);
	forwardVector = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, rotation);
	normalVector = DirectX::XMVector3Normalize(normalVector);
	rightVector = DirectX::XMVector3Normalize(rightVector);
	forwardVector = DirectX::XMVector3Normalize(forwardVector);
}
void Arrow::draw()
{
	this->mesh->DrawWithMat();
}

void Arrow::update()
{
	//Set mesh pos & rot to current member variable pos/rot
	this->mesh->position = this->position;
	this->mesh->rotation = this->rotation;
	this->mesh->scale = this->scale;

	//Update constantbuffer
	this->mesh->UpdateCB();
}