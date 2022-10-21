#pragma once
#include <ctime>
#include "GameObject.h"


class ItemSpawn
{
private:
	DirectX::SimpleMath::Vector3 randomizeVector();
public:
	ItemSpawn();
	void randomizeItemPos(GameObject* object);
};