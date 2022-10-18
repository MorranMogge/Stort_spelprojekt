#include "stdafx.h"
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
    //float x = (objectPosition.x - planetCenterPoint.x);
    //float y = (objectPosition.y - planetCenterPoint.y);
    //float z = (objectPosition.z - planetCenterPoint.z);
    //float length = sqrt(x * x + y * y + z * z);
    //float factor = gravityConstant / length;
    //objectPosition.x *= -factor;
    //objectPosition.y *= -factor;
    //objectPosition.z *= -factor;

    DirectX::SimpleMath::Vector3 objPos(objectPosition);
    DirectX::SimpleMath::Vector3 planetCenter(planetCenterPoint.x, planetCenterPoint.y, planetCenterPoint.z);
    DirectX::SimpleMath::Vector3 result((objectPosition - planetCenter) * -1);
    result.Normalize();

    //return objectPosition;
    return result;
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
