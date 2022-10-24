#include  "SpawnComponent.h"

ComponentData SpawnOneComponent(std::vector<Component>& components)
{
	ComponentData dataValue;
	Component temp;

	temp.setPosition(69.0f, 420.0f, 1337.0f);
	components.push_back(temp);

	dataValue.packetId = 13;
	dataValue.inUseBy = -1;
	dataValue.ComponentId = components.size() - 1;
	dataValue.x = 69.0f;
	dataValue.y = 420.0f;
	dataValue.z = 1337.0f;
	return dataValue;
}