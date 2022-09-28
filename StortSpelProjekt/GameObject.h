#pragma once

//#include <reactphysics3d/reactphysics3d.h>
#include "Mesh.h"

class PhysicsComponent;

class GameObject
{
private:
	Mesh* mesh;
	int objectID;
	PhysicsComponent* physComp;
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
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMFLOAT3 getRot() const;
	DirectX::XMFLOAT3 getScale() const;
	Bound* getBounds() const;
	void setPhysComp(PhysicsComponent* comp);
	PhysicsComponent* getPhysComp()const;
	void updateBuffer();
	void setMesh(std::string meshPath);
	void setMesh(Mesh*);
	bool withinRadious(GameObject *object ,float radius) const;
	virtual void draw();
};
