#include "GravityField.h"
#include "DirectXMathHelper.h"

GravityField::GravityField(float gravityConstant, DirectX::XMFLOAT3 planetCenterPoint)
    :gravityConstant(gravityConstant), planetCenterPoint(planetCenterPoint)
{
}

GravityField::~GravityField()
{
}

DirectX::XMFLOAT3 GravityField::calcGravFactor(DirectX::XMFLOAT3 objectPosition)
{
    float x = (objectPosition.x - planetCenterPoint.x);
    float y = (objectPosition.y - planetCenterPoint.y);
    float z = (objectPosition.z - planetCenterPoint.z);
    float length = sqrt(x * x + y * y + z * z);
    float factor = gravityConstant / length;
    objectPosition.x *= -factor;
    objectPosition.y *= -factor;
    objectPosition.z *= -factor;

    return objectPosition;
}

DirectX::XMFLOAT3 GravityField::calcGravFactor(DirectX::SimpleMath::Vector3 objectPosition)
{
    float x = (objectPosition.x - planetCenterPoint.x);
    float y = (objectPosition.y - planetCenterPoint.y);
    float z = (objectPosition.z - planetCenterPoint.z);
    float length = sqrt(x * x + y * y + z * z);
    float factor = gravityConstant / length;
    objectPosition.x *= -factor;
    objectPosition.y *= -factor;
    objectPosition.z *= -factor;

    return objectPosition;
}
