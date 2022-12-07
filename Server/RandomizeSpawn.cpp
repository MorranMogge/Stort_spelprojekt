#pragma once
#include "RandomizeSpawn.h"
#include "../DirectXTK-main/Inc/SimpleMath.h"
#include <iostream>

DirectX::XMFLOAT3 randomizeObjectPos(std::vector<Planet*> planets)
{
    DirectX::XMFLOAT3 position(0.f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 randomPos = {};

    int planetIndex = rand() % (int)(planets.size());

    float xPos = (float)(rand() % 201 - 100);
    float yPos = (float)(rand() % 201 - 100);
    float zPos = (float)(rand() % 201 - 100);

    randomPos.x = xPos;
    randomPos.y = yPos;
    randomPos.z = zPos;

    randomPos.Normalize();
    randomPos *= planets[planetIndex]->getSize() + 10.f;

    position.x = randomPos.x + planets[planetIndex]->getPlanetPosition().x;
    position.y = randomPos.y + planets[planetIndex]->getPlanetPosition().y;
    position.z = randomPos.z + planets[planetIndex]->getPlanetPosition().z;

    std::cout << "X: " << position.x << "\nY: " << position.y << "\nZ: " << position.z << "\n";

    return position;
}