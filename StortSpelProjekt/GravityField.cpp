#include "stdafx.h"
#include "GravityField.h"
#include "DirectXMathHelper.h"

GravityField::GravityField(const float& gravityConstant, const DirectX::XMFLOAT3& planetCenterPoint, const float& radius)
    :gravityConstant(gravityConstant), planetCenterPoint(planetCenterPoint), radius(radius)
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
    newNormalizeXMFLOAT3(objectPosition);
    scalarMultiplicationXMFLOAT3(-gravityConstant, objectPosition);
    /*float length = sqrt(x * x + y * y + z * z);
    float factor = gravityConstant / length;
    objectPosition.x *= -factor;
    objectPosition.y *= -factor;
    objectPosition.z *= -factor;*/

    //return objectPosition;

    using namespace DirectX::SimpleMath;

    Vector3 result((Vector3(objectPosition) - planetCenterPoint) * -1);
    result.Normalize();
    return result;
}

DirectX::XMFLOAT3 GravityField::calcGravFactor(const DirectX::SimpleMath::Vector3& objectPosition)
{
    DirectX::XMFLOAT3 gravFactor = objectPosition;
    gravFactor.x -= planetCenterPoint.x;
    gravFactor.y -= planetCenterPoint.y;
    gravFactor.z -= planetCenterPoint.z;
    newNormalizeXMFLOAT3(gravFactor);
    scalarMultiplicationXMFLOAT3(-gravityConstant, gravFactor);
   /* float length = sqrt(x * x + y * y + z * z);
    float factor = gravityConstant / length;
    objectPosition.x *= -factor;
    objectPosition.y *= -factor;
    objectPosition.z *= -factor;*/

    return gravFactor;
}
