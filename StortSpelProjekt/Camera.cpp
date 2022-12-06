#include "stdafx.h"
#include "Camera.h"
#include "Input.h"
#include "DirectXMathHelper.h"
#include <string>
using namespace DirectX;

void Camera::updateCamera()
{
	viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, (float)GPU::windowWidth / (float)GPU::windowHeight, 0.1f, 800.0f);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	XMStoreFloat3(&position, this->cameraPos);
	positionBuffer.getData().pos = position;
	positionBuffer.getData().padding = 0;
	this->upVectorBuffer.getData().pos = DirectX::SimpleMath::Vector3(upVector);
	this->upVectorBuffer.getData().padding = 0;

	this->upVectorBuffer.applyData();
	this->positionBuffer.applyData();
	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::Camera()
{
	this->position = { 0.0f, 0.0f, 0.0f };
	playerRotationMX = XMMatrixIdentity();

	cameraBuffer.Initialize(GPU::device, GPU::immediateContext);
	positionBuffer.Initialize(GPU::device, GPU::immediateContext);
	upVectorBuffer.Initialize(GPU::device, GPU::immediateContext);

	XMFLOAT3 position(0, 0, 0);
	XMStoreFloat3(&position, this->cameraPos);
	positionBuffer.getData().pos = position;
	positionBuffer.getData().padding = 0;
	
	this->upVectorBuffer.getData().pos = DirectX::SimpleMath::Vector3(upVector);
	this->upVectorBuffer.getData().padding = 0;
		
	viewMatrix = DirectX::XMMatrixLookAtLH(cameraPos, lookAtPos, upVector);
	projMatrix = DirectX::XMMatrixPerspectiveFovLH(0.8f, (float)GPU::windowWidth / (float)GPU::windowHeight, 0.1f, 800.0f);
	cameraBuffer.getData().viewProjMX = viewMatrix * projMatrix;
	cameraBuffer.getData().viewProjMX = XMMatrixTranspose(cameraBuffer.getData().viewProjMX);

	this->upVectorBuffer.applyData();
	this->positionBuffer.applyData();
	cameraBuffer.applyData();
	GPU::immediateContext->VSSetConstantBuffers(1, 1, cameraBuffer.getReferenceOf());
}

Camera::~Camera()
{
}

void Camera::moveCamera(Player* player, const float& deltaTime)
{
	this->playerRotationMX = player->getRotationMX();
	this->playerPosition = player->getPosV3();
	this->playerSpeed = player->getSpeed();

	//Actual camera
	upVector = XMVector3TransformCoord(DEFAULT_UP, playerRotationMX);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, playerRotationMX);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, playerRotationMX);
	lookAtPos = playerPosition;
	cameraPos = playerPosition + upVector * 60.f - forwardVector * 50.f;
	logicalPos = cameraPos;

	//Changing FOV if player moving faster
	if (XMVector3NotEqual(cameraPos, oldCameraPos))
	{
		if (playerSpeed < 26.f) minFOV = 0.76f;
		else if (playerSpeed < 38.f) minFOV = 0.7f;
		else minFOV = 0.65f;

		if (fieldOfView > (minFOV + 0.01f)) fieldOfView -= deltaTime * 0.1f;
		else if (fieldOfView < (minFOV - 0.01f))  fieldOfView += deltaTime * 0.1f;
	}
	else
	{
		maxFOV = 0.75f;
		if (fieldOfView < maxFOV) fieldOfView += deltaTime * 1.5f;
	}

	oldCameraPos = cameraPos;
	updateCamera();
}

void Camera::moveVelocity(Player* player, const float& deltaTime)
{
	this->playerSpeed = player->getSpeed();
	this->playerPosition = player->getPosV3();
	this->playerRotationMX = player->getRotationMX();

	//Actual camera
	logicalUp = XMVector3TransformCoord(DEFAULT_UP, playerRotationMX);
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, playerRotationMX);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, playerRotationMX);
	lookAtPos = playerPosition;
	logicalPos = playerPosition + logicalUp * 50.f - forwardVector * 70.f;

	//The showing camera
	velocityVector = XMVectorSubtract(logicalPos, cameraPos);
	cameraPos += velocityVector * deltaTime * 6.f;
	velocityVector = XMVectorSubtract(logicalUp, upVector);
	upVector += velocityVector * deltaTime * 6.f;

	//Changing FOV if player moving faster
	if (XMVector3NotEqual(cameraPos, oldCameraPos))
	{
		if (playerSpeed < 26.f) minFOV = 0.76f;
		else if (playerSpeed < 38.f) minFOV = 0.7f;
		else minFOV = 0.65f;

		if (fieldOfView > (minFOV + 0.03f)) fieldOfView -= deltaTime * 0.1f;
		else if (fieldOfView < (minFOV - 0.03f))  fieldOfView += deltaTime * 0.1f;
	}
	else
	{
		maxFOV = 0.75f;
		if (fieldOfView < maxFOV) fieldOfView += deltaTime * 1.5f;
	}

	oldCameraPos = cameraPos;
	updateCamera();
}

void Camera::collisionCamera(Player* player, const std::vector<Planet*>& planets, const float& deltaTime)
{
	this->playerRotationMX = player->getRotationMX();
	this->playerPosition = player->getPosV3();
	this->playerSpeed = player->getSpeed();

	//Actual camera
	logicalUp = XMVector3TransformCoord(DEFAULT_UP, playerRotationMX); // DO NOT NEED TO CHECK THE PLANET IM STANDING ON
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, playerRotationMX);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, playerRotationMX);
	lookAtPos = playerPosition;
	logicalPos = playerPosition + logicalUp * 50.f - forwardVector * 70.f;
	index = -1;

	//Checking collision with planets
	for (int i = 0; i < planets.size(); i++)
	{
		//Measuring distances with an offset
		planetVector = DirectX::XMVectorSet(planets[i]->getSize(), planets[i]->getSize(), planets[i]->getSize(), 0.0f);
		cameraVector = XMVectorSubtract(planets[i]->getPlanetPosition(), logicalPos);
		cameraVector = XMVector3Length(cameraVector);

		//It collided with planet, it goes away
		cameraVector *= 0.6f;
		if  (XMVector3LessOrEqual(cameraVector, planetVector))
		{
			index = i;
		}

		//It collided with planet, it ducks down
		cameraVector *= 0.9f;
		if (XMVector3LessOrEqual(cameraVector, planetVector))
		{
			logicalPos -= logicalUp * 20.f;
			logicalPos += forwardVector * 10.f;
		}
	}

	//The showing camera
	velocityVector = XMVectorSubtract(logicalPos, cameraPos);
	cameraPos += velocityVector * deltaTime * 6.f;
	velocityVector = XMVectorSubtract(logicalUp, upVector);
	upVector += velocityVector * deltaTime * 6.f;

	//Changing FOV if player moving faster
	if (XMVector3NotEqual(cameraPos, oldCameraPos))
	{
		if (playerSpeed < 26.f) minFOV = 0.76f;
		else if (playerSpeed < 38.f) minFOV = 0.65f;
		else minFOV = 0.6f;

		if (fieldOfView > (minFOV + 0.01f)) fieldOfView -= deltaTime * 0.1f;
		else if (fieldOfView < (minFOV - 0.01f))  fieldOfView += deltaTime * 0.1f;
	}
	else
	{
		maxFOV = 0.75f;
		if (fieldOfView < maxFOV) fieldOfView += deltaTime * 1.8f;
	}

	oldCameraPos = cameraPos;
	updateCamera();
}

void Camera::winScene(const DirectX::XMVECTOR& shipPosition, const DirectX::XMMATRIX& shipRotation)
{
	playerRotationMX = shipRotation;
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, playerRotationMX);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, playerRotationMX);
	upVector = XMVector3TransformCoord(DEFAULT_UP, playerRotationMX);

	cameraPos = shipPosition + upVector * 40.f - forwardVector * 50.f;
	lookAtPos = shipPosition;
	updateCamera();
}

void Camera::landingMinigameScene(const Planet* planet, const DirectX::XMVECTOR& shipPosition, const DirectX::XMMATRIX& shipRotation)
{
	rightVector = XMVector3TransformCoord(DEFAULT_RIGHT, shipRotation);
	forwardVector = XMVector3TransformCoord(DEFAULT_FORWARD, shipRotation);
	upVector = XMVector3TransformCoord(DEFAULT_UP, shipRotation);

	DirectX::XMVECTOR middlePos = ((DirectX::XMVECTOR)planet->getPlanetPosition() + shipPosition) / 2.f;
	middlePos += 1.25f*upVector*(DirectX::XMVECTOR)DirectX::SimpleMath::Vector3(planet->getSize() / 2.f, planet->getSize() / 2.f, planet->getSize() / 2.f);
	lookAtPos = middlePos;
	cameraPos = middlePos - forwardVector * 1.25f/fieldOfView * getLength((DirectX::SimpleMath::Vector3)middlePos);
	updateCamera();
}

DirectX::XMVECTOR Camera::getForwardVector() const
{
	return this->forwardVector;
}

DirectX::XMVECTOR Camera::getRightVector() const
{
	return this->rightVector;
}

DirectX::XMVECTOR Camera::getUpVector() const
{
	return this->upVector;
}

DirectX::XMVECTOR Camera::getPosition() const
{
	return this->cameraPos;
}

DirectX::XMVECTOR Camera::getRealPosition() const
{
	return this->cameraPos;
}

int Camera::getCollidedWith() const
{
	return this->index;
}

void Camera::setPosition(const DirectX::XMFLOAT3& position)
{
	this->cameraPos = DirectX::XMVectorSet(position.x, position.y, position.z, 1.f);
	this->lookAtPos = cameraPos + DirectX::XMVectorSet(0.f, 0.f, 10.f, 1.f);
	this->updateCamera();
}

void Camera::setCameraLookAt(const DirectX::XMFLOAT3& position)
{
	if (DirectX::XMVector3Equal(this->cameraPos, DirectX::XMVectorSet(position.x, position.y, position.z, 1.f))) return;
	this->lookAtPos = DirectX::XMVectorSet(position.x, position.y, position.z, 1.f);
	this->updateCamera();
}

void Camera::setRotToStart()
{
	rightVector = DEFAULT_RIGHT;
	forwardVector = DEFAULT_FORWARD;
	upVector = DEFAULT_UP;
}

void Camera::VSbindPositionBuffer(const int& slot)
{
	GPU::immediateContext->VSSetConstantBuffers(slot, 1, this->positionBuffer.getReferenceOf());
}

void Camera::VSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->VSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}

void Camera::PSbindPositionBuffer(const int& slot)
{
	GPU::immediateContext->PSSetConstantBuffers(slot, 1, this->positionBuffer.getReferenceOf());
}

void Camera::PSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->PSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}

void Camera::GSbindPositionBuffer(const int& slot)
{
	GPU::immediateContext->GSSetConstantBuffers(slot, 1, this->positionBuffer.getReferenceOf());
}

void Camera::GSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->GSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}

void Camera::GSbindUpBuffer(const int& slot)
{
	GPU::immediateContext->GSSetConstantBuffers(slot, 1, this->upVectorBuffer.getReferenceOf());
}

void Camera::CSbindUpBuffer(const int& slot)
{
	GPU::immediateContext->CSSetConstantBuffers(slot, 1, this->upVectorBuffer.getReferenceOf());
}

void Camera::DSbindViewBuffer(const int& slot)
{
	GPU::immediateContext->DSSetConstantBuffers(slot, 1, this->cameraBuffer.getReferenceOf());
}
