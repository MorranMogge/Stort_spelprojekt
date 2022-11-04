#include "stdafx.h"
#include "Arrow.h"
using namespace DirectX;

Arrow::Arrow(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot)
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
	this->rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMVectorSet(rot.x, rot.y, rot.z, 1.0f));
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

DirectX::XMVECTOR Arrow::getPosition() const
{
	return this->position;
}

DirectX::XMVECTOR Arrow::getArrowVector() const
{
	return this->arrowVector;
}

void Arrow::pointDirection(const DirectX::XMVECTOR& compPosition)
{
	arrowVector = XMVectorSubtract(compPosition, this->position);
}

void Arrow::changeDirection(const DirectX::XMVECTOR& cameraPosition, const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraUp, const DirectX::XMMATRIX& cameraRotation)
{
	this->position = cameraPosition - cameraUp * 40.f + cameraForward * 20.f;
	this->rotation = cameraRotation;
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