
#include "SpawnComponent.h"
#include "DirectXMathHelper.h"
#include "RandomizeSpawn.h"
#include <iostream>


SpawnComponent SpawnOneComponent(std::vector<Component>& components, std::vector<DirectX::XMFLOAT3>& spaceShipPos)
{
	SpawnComponent dataValue;
	Component temp;

	DirectX::SimpleMath::Vector3 randomPos;
	DirectX::SimpleMath::Vector3 spaceShipPos1 = normalizeXMFLOAT3(spaceShipPos[0]);
	DirectX::SimpleMath::Vector3 spaceShipPos2 = normalizeXMFLOAT3(spaceShipPos[1]);

	while (true)
	{

		DirectX::XMFLOAT3 tmp(0.f, 0.0f, 0.0f);


		tmp.x = (float)(rand() % 201 - 100);
		tmp.y = (float)(rand() % 201 - 100);
		tmp.z = (float)(rand() % 201 - 100);

		randomPos = normalizeXMFLOAT3(tmp);

		DirectX::SimpleMath::Vector3 spaceShip1DotRandomizedVec = DirectX::XMVector3Dot(randomPos, spaceShipPos1);
		DirectX::SimpleMath::Vector3 spaceShip2DotRandomizedVec = DirectX::XMVector3Dot(randomPos, spaceShipPos2);

		float res1 = spaceShip1DotRandomizedVec.x + spaceShip1DotRandomizedVec.y + spaceShip1DotRandomizedVec.z;
		float res2 = spaceShip2DotRandomizedVec.x + spaceShip2DotRandomizedVec.y + spaceShip2DotRandomizedVec.z;

		std::cout << "res1: " << std::to_string(res1) << "\n";
		std::cout << "res2: " << std::to_string(res2) << "\n";

		if (res1 > 1.20f || res1 < 0.8 && res2 > 1.20f || res2 < 0.8)
		{
			break;
		}
	}

	randomPos *= 100;

	components.push_back(temp);

	dataValue.packetId = 13;
	dataValue.ComponentId = components.size() - 1;
	std::cout << "Comp ID: " << dataValue.ComponentId << "\n";
	dataValue.x = randomPos.x;
	dataValue.y = randomPos.y;
	dataValue.z = randomPos.z;
	return dataValue;
}