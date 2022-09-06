#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include <string>

class GameObject
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	Mesh* mesh;
	int objectID;
	//float mass;
	//collider
public:
	GameObject(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	GameObject(std::string meshPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	void setPos(DirectX::XMFLOAT3 pos);
	void setRot(DirectX::XMFLOAT3 rot);
	DirectX::XMFLOAT3 getPos();
	DirectX::XMFLOAT3 getRot();
	Bound* getBounds();
	void updateBuffer();
	void setMesh(std::string meshPath);

};
