#pragma once
#include "DirectXMathHelper.h"
#include "GameObject.h"


class TestObj : public GameObject
{
private:

public:
	TestObj(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1, 1, 1));
};