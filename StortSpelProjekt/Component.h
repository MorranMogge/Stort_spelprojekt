#pragma once
#include "Item.h"

class Component : public Item
{
private:
	bool pickedUp;
public:
	Component(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	Component(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	~Component();

	void interactWithComponent();
	bool returnStatus()const;

	// Inherited via Item
	virtual void useItem(const Player* playerHoldingItem) override;
};
