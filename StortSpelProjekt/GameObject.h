#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include <string>

class GameObject
{
private:

	Mesh* mesh;
	int objectID;
	//float mass
	//collider
protected:
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMFLOAT3 rotation;//ska ändra till quaternion?
	DirectX::XMFLOAT3 scale;

public:
	GameObject(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject(std::string meshPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id, DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject();
	virtual ~GameObject();
	void movePos(DirectX::XMFLOAT3 offset);
	void setPos(DirectX::XMFLOAT3 pos);
	void setRot(DirectX::XMFLOAT3 rot);
	void setScale(DirectX::XMFLOAT3 scale);
	void setMesh(std::string meshPath);
	void setMesh(Mesh* newMesh);
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMFLOAT3 getRot() const;
	DirectX::XMFLOAT3 getScale() const;
	Bound* getBounds() const;
	void updateBuffer();
	bool withinRadious(GameObject *object ,float radius) const;
	void draw();
};
