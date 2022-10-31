#pragma once
#include "Item.h"

class Component : public Item
{
private:
	bool pickedUp;

public:
	Component(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	Component(Mesh* mesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	~Component();

	void interactWithComponent();
	bool returnStatus()const;

	// Inherited via Item
	virtual void useItem() override;
};
