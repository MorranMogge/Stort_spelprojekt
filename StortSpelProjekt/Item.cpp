#include "stdafx.h"
#include "Item.h"

Item::Item(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int onlineId, int onlineType)
	:GameObject(useMesh, pos, rot, id)
{
	this->onlineType = onlineType;
	this->onlineId = onlineId;
}

Item::Item(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int onlineId, int onlineType)
	:GameObject(objectPath, pos, rot, id)
{
	this->onlineType = onlineType;
	this->onlineId = onlineId;
}

Item::~Item()
{
}

void Item::throwItem()
{
}

int Item::getOnlineType() const
{
	return onlineType;
}

int Item::getOnlineId() const
{
	return onlineId;
}
