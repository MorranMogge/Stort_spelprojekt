#pragma once
#include "GameObject.h"


//Lägga in mutex så att inte flera spelar kan ta samma item.
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