#pragma once

#include "Mesh.h"

class PhysicsComponent;

class GameObject
{
private:
	Mesh* mesh;
	int objectID;
	reactphysics3d::Quaternion reactQuaternion;
	DirectX::SimpleMath::Quaternion dx11Quaternion;
	ID3D11ShaderResourceView* srv;
protected:
	PhysicsComponent* physComp;
	DirectX::SimpleMath::Vector3 position;
	DirectX::XMMATRIX rotation;
	DirectX::XMFLOAT3 scale;


public:
	GameObject(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject(const std::string& meshPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject(ID3D11Buffer* vBuff, ID3D11Buffer* iBuff, std::vector<int>& subMeshRanges, std::vector<int>& verticies, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
	GameObject();
	virtual ~GameObject();
	void movePos(const DirectX::XMFLOAT3& offset);
	void setPos(const DirectX::XMFLOAT3& pos);
	void setRot(const DirectX::XMVECTOR& rot);
	void setRot(const DirectX::XMFLOAT3& rot);
	void setScale(const DirectX::XMFLOAT3& scale);
	void setMesh(const std::string& meshPath);
	void setMesh(Mesh* inMesh);
	DirectX::XMFLOAT3 getPos() const;
	DirectX::SimpleMath::Vector3 getPosV3()const;
	DirectX::XMMATRIX getRot() const;
	DirectX::XMFLOAT3 getScale() const;
	Bound* getBounds() const;
	void setPhysComp(PhysicsComponent* comp);
	PhysicsComponent* getPhysComp()const;
	void updateBuffer();
	int getId();
	bool withinBox(GameObject *object ,float x, float y, float z) const;
	bool withinRadious(GameObject* object, float radius) const;
	virtual void draw();
	virtual void update(); //This updates the hitboxes
	void tmpDraw();
	void setSrv(ID3D11ShaderResourceView* srv);
};
