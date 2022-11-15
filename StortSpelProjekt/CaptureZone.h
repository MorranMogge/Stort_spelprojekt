#pragma once
#include "GameObject.h"
#include "ConstantBufferNew.h"

class CaptureZone
{
private:
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;
	GameObject* capMesh;
	Mesh* tempMesh;

public:
	CaptureZone(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 rotation, DirectX::SimpleMath::Vector3 scale, DirectX::SimpleMath::Vector3 color = DirectX::XMFLOAT3(0.0f, 0.55f, 0.75f));
	~CaptureZone();
	void setColor(const DirectX::SimpleMath::Vector3 & color);
	void setPosition(const DirectX::SimpleMath::Vector3 & position);
	void setRotation(const DirectX::SimpleMath::Vector3 & rotation);
	void setScale(const DirectX::SimpleMath::Vector3 & scale);
	void draw();

};