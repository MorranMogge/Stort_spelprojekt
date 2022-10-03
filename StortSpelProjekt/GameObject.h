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
	GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject();
	virtual ~GameObject();
	void movePos(const DirectX::XMFLOAT3& offset);
	void setPos(const DirectX::XMFLOAT3& pos);
	void setRot(const DirectX::XMFLOAT3& rot);
	void setScale(const DirectX::XMFLOAT3& scale);
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMFLOAT3 getRot() const;
	DirectX::XMFLOAT3 getScale() const;
	Bound* getBounds() const;
	void updateBuffer();
	void setMesh(const std::string& meshPath);
	void setMesh(Mesh*);
	bool withinRadious(GameObject *object , const float& radius) const;
	virtual void draw();
};
