#pragma once
#include "GameObject.h"


//Lägga in mutex så att inte flera spelar kan ta samma item.
class Item : public GameObject
{
private:
	int onlineType;
	int onlineId;
public:
	Item(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int onlineId, int onlineType);
	Item(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int onlineId, int onlineType);
	virtual ~Item();
	virtual void useItem() = 0;
	//virtual void useItem();
	void throwItem();
	int getOnlineType()const;
	int getOnlineId()const;
};