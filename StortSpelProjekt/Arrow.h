#pragma once
#include "Mesh.h"

class Arrow
{
private:
	Mesh* mesh;
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 resultVector;
	DirectX::XMMATRIX rotation;
	DirectX::XMFLOAT3 scale;

	const DirectX::XMVECTOR DEFAULT_UP = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::SimpleMath::Vector3 normalVector = DEFAULT_UP;
	DirectX::SimpleMath::Vector3 rightVector = DEFAULT_RIGHT;
	DirectX::SimpleMath::Vector3 forwardVector = DEFAULT_FORWARD;
	DirectX::XMVECTOR arrowVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

public:
	Arrow(const std::string& objectPath, const DirectX::XMFLOAT3& pos);
	~Arrow();

	DirectX::XMVECTOR getUpVectorArrow() const;
	DirectX::XMVECTOR getForwardVectorArrow() const;
	DirectX::XMVECTOR getRightVectorArrow() const;
	DirectX::XMVECTOR getArrowVectorArrow() const;

	DirectX::XMVECTOR getPosition() const;
	void moveWithCamera(const DirectX::XMVECTOR& cameraPosition, const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraUp, const DirectX::XMMATRIX& cameraRotation);
	void showDirection(const DirectX::XMVECTOR& goalPosition, const DirectX::XMVECTOR& playerPosition, const DirectX::XMFLOAT3& gravity);
	void draw();
	void update();
};