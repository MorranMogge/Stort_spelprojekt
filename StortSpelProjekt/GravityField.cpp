#include "stdafx.h"
#include "GravityField.h"
#include "DirectXMathHelper.h"

GravityField::GravityField(const float& gravityConstant, const DirectX::XMFLOAT3& planetCenterPoint, const float& radius)
    :gravityConstant(gravityConstant), planetCenterPoint(planetCenterPoint), radius(radius), shape(0)
{
}

GravityField::~GravityField()
{
}

void GravityField::setCenterpoint(const DirectX::XMFLOAT3& newPos)
{
    this->planetCenterPoint = newPos;
}

void GravityField::setShape(const PlanetShape& shape, const DirectX::SimpleMath::Vector3& cubeScale)
{
    this->shape = 1;
    this->cubeScale = cubeScale;
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

    DirectX::SimpleMath::Vector3 newStuff;
    int closestLength;
    static DirectX::SimpleMath::Vector3 lengths[6];

    switch (shape)
    {
    case 0:
        break;
    case 1:
        for (int i = 0; i < 6; i++)
        {
            lengths[i] = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);
        }
        newStuff = objectPosition - this->planetCenterPoint;
        lengths[0] = newStuff - DirectX::SimpleMath::Vector3(cubeScale.x, 0.f, 0.f);
        lengths[1] = newStuff - DirectX::SimpleMath::Vector3(-cubeScale.x, 0.f, 0.f);
        lengths[2] = newStuff - DirectX::SimpleMath::Vector3(0.f, cubeScale.x, 0.f);
        lengths[3] = newStuff - DirectX::SimpleMath::Vector3(0.f, -cubeScale.x, 0.f);
        lengths[4] = newStuff - DirectX::SimpleMath::Vector3(0.f, 0.f, cubeScale.x);
        lengths[5] = newStuff - DirectX::SimpleMath::Vector3(0.f, 0.f, -cubeScale.x);
        closestLength = 0;
        for (int i = 1; i < 6; i++)
        {
            if (getLength(lengths[closestLength]) > getLength(lengths[i])) closestLength = i;
        }
        gravFactor = lengths[closestLength];
        scalarMultiplicationXMFLOAT3(-gravityConstant, gravFactor);
        break;
    default:
        break;
    }
   /* float length = sqrt(x * x + y * y + z * z);
    float factor = gravityConstant / length;
    objectPosition.x *= -factor;
    objectPosition.y *= -factor;
    objectPosition.z *= -factor;*/

    return gravFactor;
}
