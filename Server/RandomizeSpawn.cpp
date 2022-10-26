#pragma once
#include "RandomizeSpawn.h"
#include "../DirectXTK-main/Inc/SimpleMath.h"

DirectX::XMFLOAT3 randomizeObjectPos()
{
    DirectX::XMFLOAT3 position(0.f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 randomPos = {};

    int xPos = rand() % 201 - 100;
    int yPos = rand() % 201 - 100;
    int zPos = rand() % 201 - 100;

    randomPos.x = xPos;
    randomPos.y = yPos;
    randomPos.z = zPos;

    randomPos.Normalize();
    randomPos *= 100;

    position.x = randomPos.x;
    position.y = randomPos.y;
    position.z = randomPos.z;

    return position;
}
