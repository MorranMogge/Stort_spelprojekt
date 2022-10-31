
#include "SpawnComponent.h"

#include "RandomizeSpawn.h"

SpawnComponent SpawnOneComponent(std::vector<Component>& components, int& componentId)
{
	SpawnComponent dataValue;
	Component temp;

	DirectX::SimpleMath::Vector3 randomPos = randomizeObjectPos();

	components.push_back(temp);
	
	dataValue.packetId = 13;
	std::cout << "comp id FÖRE: " << std::to_string(componentId) << std::endl;
	dataValue.ComponentId = componentId++;
	std::cout << "comp id EFTER: " << std::to_string(componentId) << std::endl;
	dataValue.x = randomPos.x;
	dataValue.y = randomPos.y;
	dataValue.z = randomPos.z;
	return dataValue;
}