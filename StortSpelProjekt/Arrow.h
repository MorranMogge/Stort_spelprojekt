#pragma once
#include "Mesh.h"

class Arrow
{
private:
	Mesh* mesh;
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMMATRIX rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMVECTOR arrowVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

public:
	Arrow(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
	~Arrow();

	DirectX::XMVECTOR getPosition() const;
	DirectX::XMVECTOR getArrowVector() const;
	void pointDirection(const DirectX::XMVECTOR& compPosition);
	void changeDirection(const DirectX::XMVECTOR& cameraPosition, const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraUp, const DirectX::XMMATRIX& cameraRotation);
	void draw();
	void update();
};