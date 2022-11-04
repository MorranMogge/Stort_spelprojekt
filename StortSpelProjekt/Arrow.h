#pragma once
#include "Mesh.h"

class Arrow
{
private:
	Mesh* mesh;
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMMATRIX rotation;
	DirectX::XMFLOAT3 scale;

public:
	Arrow(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot);
	~Arrow();

	DirectX::XMVECTOR getPosition() const;
	void changeDirection(const DirectX::XMVECTOR& cameraPosition, const DirectX::XMVECTOR& cameraForward, const DirectX::XMVECTOR& cameraUp, const DirectX::XMMATRIX& cameraRotation);
	void draw();
	void update();
};