#include "stdafx.h"
#include "Item.h"

Item::Item(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(useMesh, pos, rot, id)
{
}

Item::Item(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id)
	:GameObject(objectPath, pos, rot, id)
{
}

Item::~Item()
{
}

void Item::throwItem()
{
}
