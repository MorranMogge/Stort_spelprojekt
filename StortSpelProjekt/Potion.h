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
	Potion(Mesh* useMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	Potion(std::string objectPath, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, int id);
	
	virtual void useItem() override;
	int getRestore() const;
	void setPlayerptr(Player* ptr);
	virtual ~Potion();


};
