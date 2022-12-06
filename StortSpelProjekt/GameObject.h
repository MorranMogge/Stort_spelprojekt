#pragma once

#include "Mesh.h"
#include "GravityField.h"

class PhysicsComponent;

enum ObjID
{
	PLANET,
	PLAYER,
	ROCKET,
	BAT,
	POTION,
	GRENADE,
	COMPONENT,
	MOVABLE
};

class GameObject
{
private:

	int objectID;
	reactphysics3d::Quaternion reactQuaternion;
	DirectX::SimpleMath::Quaternion dx11Quaternion;
	ID3D11ShaderResourceView* srv;
	GravityField* activeField;

protected:
	Mesh* mesh;
	PhysicsComponent* physComp;
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMMATRIX rotation;
	DirectX::XMFLOAT3 scale;

public:
	GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field = nullptr, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, GravityField* field = nullptr, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject();
	virtual ~GameObject();
	void movePos(const DirectX::XMFLOAT3& offset);
	void setPos(const DirectX::XMFLOAT3& pos);
	void setRot(const DirectX::XMVECTOR& rot);
	void setRot(const DirectX::XMFLOAT3& rot);
	void setRot(const DirectX::SimpleMath::Quaternion& rot);
	void setScale(const DirectX::XMFLOAT3& scale);
	void setScale(const float& scale);
	void setMesh(const std::string& meshPath);
	void setMesh(Mesh* inMesh);
	void setGravityField(GravityField* field);
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMMATRIX getRot() const;
	DirectX::XMFLOAT3 getRotXM() const;
	DirectX::XMFLOAT3 getScale() const;
	DirectX::XMFLOAT4X4 getMatrix() const;

	void setMatrix(DirectX::XMFLOAT4X4 matrix);
	void updateMatrixOnline();
	Bound* getBounds() const;
	void setPhysComp(PhysicsComponent* comp);
	PhysicsComponent* getPhysComp()const;
	void orientToUpDirection();
	DirectX::XMFLOAT3 getRotOrientedToGrav() const; //from gravity field
	DirectX::XMFLOAT3 getUpDirection() const;
	void updateBuffer();
	int getId();
	bool withinBox(GameObject* object, float x, float y, float z) const;
	bool withinRadious(GameObject* object, float radius) const;
	void updatePhysCompRotation();
	virtual void draw();
	void updateRotation();
	virtual void update(); //This updates the hitboxes
	void tmpDraw(UINT stride = 32U);
	void tmpDraw(std::vector<ID3D11ShaderResourceView*>& allTextures, UINT stride = 32U);
	void setSrv(ID3D11ShaderResourceView* srv);
};