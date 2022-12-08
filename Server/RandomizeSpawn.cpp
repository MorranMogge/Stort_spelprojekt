#pragma once
#include "RandomizeSpawn.h"
#include "../DirectXTK-main/Inc/SimpleMath.h"
#include <iostream>

DirectX::XMFLOAT3 randomizeObjectPos(std::vector<Planet*> planets)
{
    DirectX::SimpleMath::Vector3 position(0.f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 randomPos = {};

    float xPos = (float)(rand() % 201 - 100);
    float yPos = (float)(rand() % 201 - 100);
    float zPos = (float)(rand() % 201 - 100);

    randomPos.x = xPos;
    randomPos.y = yPos;
    randomPos.z = zPos;

    randomPos.Normalize();
    
    int planetIndex = rand() % (int)(planets.size());
    randomPos *= (planets[planetIndex]->getSize() + 10.f);

    position = randomPos + planets[planetIndex]->getPlanetPosition();
   
    DirectX::SimpleMath::Vector3 testingIfBad;
    for (int i = 0; i < planets.size(); i++)
    {
        testingIfBad = position - planets[i]->getPlanetPosition();
        if (testingIfBad.Length() <= planets[i]->getSize()) std::cout << "React is gonna cry\n";
    }

    return position;
}
