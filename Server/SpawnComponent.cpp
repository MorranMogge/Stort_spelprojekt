#include "SpawnComponent.h"
#include "RandomizeSpawn.h"

SpawnComponent SpawnOneComponent(std::vector<Component>& components)
{
	SpawnComponent dataValue;
	Component temp;

	DirectX::SimpleMath::Vector3 randomPos = randomizeObjectPos();
	temp.setPosition(randomPos.x, randomPos.y, randomPos.z);
	components.push_back(temp);

	dataValue.packetId = 13;
	dataValue.ComponentId = components.size() - 1;
	dataValue.x = randomPos.x;
	dataValue.y = randomPos.y;
	dataValue.z = randomPos.z;
	return dataValue;
}