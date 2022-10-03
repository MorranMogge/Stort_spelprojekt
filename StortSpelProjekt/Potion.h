#pragma once
#include "Item.h"
#include "Player.h"

class Player;

class Potion : public Item 
{
private:
	Player* playerPtr;
	int restoringAmount;
public:
	Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id);
	
	virtual void useItem() override;
	int getRestore() const;
	void setPlayerptr(Player* ptr);
	virtual ~Potion();


};
