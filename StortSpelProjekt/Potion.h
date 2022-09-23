#pragma once
#include "Item.h"

class Potion : public Item 
{
public:
	Potion(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Potion(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	~Potion();
	virtual void useItem() override;




};
