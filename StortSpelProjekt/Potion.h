#pragma once
#include "Item.h"
#include "TimeStruct.h"

class Potion : public Item 
{
private:
	TimeStruct timer;
	bool timeToRun = false;

public:
	Potion(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	Potion(const std::string& objectPath, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const int& onlineId, GravityField* field = nullptr);
	virtual ~Potion();

	virtual void useItem() override;
	bool isTimeToRun();
};
