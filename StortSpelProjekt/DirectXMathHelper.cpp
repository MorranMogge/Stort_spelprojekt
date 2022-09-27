#include "DirectXMathHelper.h"

void subtractionXMFLOAT3(DirectX::XMFLOAT3& argOne, DirectX::XMFLOAT3 argTwo)
{
    argOne.x -= argTwo.x;
    argOne.y -= argTwo.y;
    argOne.z -= argTwo.z;
}

void additionXMFLOAT3(DirectX::XMFLOAT3& argOne, DirectX::XMFLOAT3 argTwo)
{
    argOne.x += argTwo.x;
    argOne.y += argTwo.y;
    argOne.z += argTwo.z;
}

DirectX::XMFLOAT3 normalizeXMFLOAT3(DirectX::XMFLOAT3 argOne)
{
    float x = argOne.x;
    float y = argOne.y;
    float z = argOne.z;
    float length = sqrt(x * x + y * y + z * z);
    if (length == 0) return argOne;
    float factor = 1 / length;
    argOne.x *= factor;
    argOne.y *= factor;
    argOne.z *= factor;
    return argOne;
}

void newNormalizeXMFLOAT3(DirectX::XMFLOAT3& argOne)
{
    float x = argOne.x;
    float y = argOne.y;
    float z = argOne.z;
    float length = sqrt(x * x + y * y + z * z);
    float factor = 1 / length;
    argOne.x *= factor;
    argOne.y *= factor;
    argOne.z *= factor;
}

void scalarMultiplicationXMFLOAT3(float scalar, DirectX::XMFLOAT3& argOne)
{
    argOne.x *= scalar;
    argOne.y *= scalar;
    argOne.z *= scalar;
}

DirectX::XMFLOAT3 getScalarMultiplicationXMFLOAT3(float scalar, DirectX::XMFLOAT3 argOne)
{
    argOne.x *= scalar;
    argOne.y *= scalar;
    argOne.z *= scalar;

    return argOne;
}

float getLength(DirectX::XMFLOAT3 argOne)
{
    return sqrt(argOne.x * argOne.x + argOne.y * argOne.y + argOne.z * argOne.z);
}
