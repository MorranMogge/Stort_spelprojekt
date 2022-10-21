#include "stdafx.h"
#include "ItemSpawn.h"

DirectX::SimpleMath::Vector3 ItemSpawn::randomizeVector()
{
	int xPos = rand() % 201 - 100;
	int yPos = rand() % 201 - 100;
	int zPos = rand() % 201 - 100;

	return DirectX::SimpleMath::Vector3(xPos, yPos, zPos);
}

ItemSpawn::ItemSpawn()
{
	srand(unsigned int(time(0)));
	
}

void ItemSpawn::randomizeItemPos(GameObject* object)
{
	DirectX::SimpleMath::Vector3 randomPos = randomizeVector();
	randomPos.Normalize();
	randomPos *= 30;

	object->setPos(randomPos);
	

	std::cout << randomPos.x << " " << randomPos.y << " " << randomPos.z << "\n";
}
