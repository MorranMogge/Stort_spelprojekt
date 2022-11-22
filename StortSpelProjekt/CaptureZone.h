#pragma once
#include "GameObject.h"
#include "ConstantBufferNew.h"

class CaptureZone: public GameObject
{
private:
	ConstantBufferNew<DirectX::XMFLOAT4> colorBuffer;
public:

	CaptureZone(Mesh* useMesh ,const DirectX::SimpleMath::Vector3 & position, const DirectX::SimpleMath::Vector3 & rotation, GravityField* field, const DirectX::SimpleMath::Vector3 & scale = DirectX::XMFLOAT3(1, 1, 1), const DirectX::SimpleMath::Vector3 & color = DirectX::XMFLOAT3(0.0f, 0.55f, 0.75f));
	//~CaptureZone();
	void setColor(const DirectX::SimpleMath::Vector3 & color);
	bool detectedObject(GameObject* objectToCheck) const;
	void drawFresnel();
};