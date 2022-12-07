#pragma once
#include "RandomizeSpawn.h"
#include "../DirectXTK-main/Inc/SimpleMath.h"

DirectX::XMFLOAT3 randomizeObjectPos()
{
    DirectX::XMFLOAT3 position(0.f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector3 randomPos = {};

    float xPos = (float)(rand() % 201 - 100);
    float yPos = (float)(rand() % 201 - 100);
    float zPos = (float)(rand() % 201 - 100);

    randomPos.x = xPos;
    randomPos.y = yPos;
    randomPos.z = zPos;

    randomPos.Normalize();
    randomPos *= 50;

    position.x = randomPos.x;
    position.y = randomPos.y;
    position.z = randomPos.z;

    return position;
}
