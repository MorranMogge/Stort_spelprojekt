#include "TestObj.h"
#include "GameObject.h"

TestObj::TestObj(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rot, const int& id, const DirectX::XMFLOAT3& scale)
	:GameObject(pos,rot,id,scale)
{

}