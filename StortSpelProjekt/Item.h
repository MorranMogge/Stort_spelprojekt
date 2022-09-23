#pragma once
#include "GameObject.h"

class Item : public GameObject
{
private:


public:
	Item(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Item(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);

	virtual void useItem() = 0;
	//virtual void useItem();
	void throwItem();
};