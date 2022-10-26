#pragma once
#include "Item.h"

class Player;

class Potion : public Item 
{
private:
	Player* playerPtr;
	int restoringAmount;
public:
	Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId);
	Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId);
	
	virtual void useItem() override;
	int getRestore() const;
	void setPlayerptr(Player* ptr);
	virtual ~Potion();


};
