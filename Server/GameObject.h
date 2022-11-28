#pragma once
#include <SimpleMath.h>

class PhysicsComponent;

class GameObject
{
private:

	int objectID;

protected:
	PhysicsComponent* physComp;
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMMATRIX rotation;
	DirectX::XMFLOAT3 scale;

public:
	GameObject(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject();
	void movePos(const DirectX::XMFLOAT3& offset);
	void setPos(const DirectX::XMFLOAT3& pos);
	void setRot(const DirectX::XMVECTOR& rot);
	void setRot(const DirectX::XMFLOAT3& rot);
	void setScale(const DirectX::XMFLOAT3& scale);
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMMATRIX getRot() const;
	DirectX::XMFLOAT3 getRotXM() const;
	DirectX::XMFLOAT3 getScale() const;
	DirectX::XMFLOAT4X4 getMatrix() const;

	void setMatrix(DirectX::XMFLOAT4X4 matrix);

	void setPhysComp(PhysicsComponent* comp);
	PhysicsComponent* getPhysComp()const;
	int getId();
	bool withinBox(GameObject* object, float x, float y, float z) const;
	bool withinRadious(GameObject* object, float radius) const;
	void updatePhysCompRotation();
	void updateRotation();
	virtual void update(); //This updates the hitboxes
};