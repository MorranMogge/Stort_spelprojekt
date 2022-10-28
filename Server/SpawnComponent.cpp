
#include "SpawnComponent.h"

#include "RandomizeSpawn.h"

SpawnComponent SpawnOneComponent(std::vector<Component>& components, long& componentId)
{
	SpawnComponent dataValue;
	Component temp;

	DirectX::SimpleMath::Vector3 randomPos = randomizeObjectPos();

	components.push_back(temp);
	
	dataValue.packetId = 13;
	dataValue.ComponentId = componentId++;
	dataValue.x = randomPos.x;
	dataValue.y = randomPos.y;
	dataValue.z = randomPos.z;
	return dataValue;
}