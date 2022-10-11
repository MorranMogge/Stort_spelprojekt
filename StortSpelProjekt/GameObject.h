#pragma once

#include "Mesh.h"

class PhysicsComponent;

class GameObject
{
private:
	Mesh* mesh;
	int objectID;
	PhysicsComponent* physComp;

protected:
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

public:
	GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject();
	virtual ~GameObject();
	void movePos(const DirectX::XMFLOAT3& offset);
	void setPos(const DirectX::XMFLOAT3& pos);
	void setRot(const DirectX::XMVECTOR& rot);
	void setRot(const DirectX::XMFLOAT3& rot);
	void setScale(DirectX::XMFLOAT3 scale);
	void setMesh(const std::string& meshPath);
	void setMesh(Mesh* inMesh);
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMFLOAT3 getRot() const;
	DirectX::XMFLOAT3 getScale() const;
	Bound* getBounds() const;
	void setPhysComp(PhysicsComponent* comp);
	PhysicsComponent* getPhysComp()const;
	void updateBuffer();
	bool withinBox(GameObject *object ,float x, float y, float z) const;
	bool withinRadious(GameObject* object, float radius) const;
	virtual void draw();
	virtual void update(); //This updates the hitboxes
};
