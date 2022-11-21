#pragma once
#include "Component.h"
#include "PacketsDataTypes.h"
#include <vector>

SpawnComponent SpawnOneComponent(std::vector<Component>& components, std::vector<DirectX::XMFLOAT3>& spaceShipPos);
//SpawnComponent SpawnOneComponent(std::vector<Item*>& itemObj, std::vector<DirectX::XMFLOAT3>& spaceShipPos);