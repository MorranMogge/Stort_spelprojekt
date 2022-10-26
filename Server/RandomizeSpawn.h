#pragma once
#include<DirectXMath.h>
#include "../DirectXTK-main/Inc/SimpleMath.h"


DirectX::SimpleMath::Vector3 randomizeObjectPos()
{
    DirectX::XMFLOAT3 position;
    DirectX::SimpleMath::Vector3 randomPos = {};

    int xPos = rand() % 201 - 100;
    int yPos = rand() % 201 - 100;
    int zPos = rand() % 201 - 100;

    position.x = xPos;
    randomPos.y = yPos;
    randomPos.z = zPos;

    randomPos.Normalize();
    randomPos *= 100;

    return randomPos;
}