#include "stdafx.h"
#include "SpaceShip.h"

SpaceShip::SpaceShip(Mesh* useMesh, const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:GameObject(useMesh, pos, rot, id, scale), components(nullptr)
{

}

SpaceShip::SpaceShip(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:GameObject("../Meshes/rocket", pos, rot, id, scale), components(nullptr)
{

}

SpaceShip::~SpaceShip()
{
	//Delete components?
}
