#include "LightHandler.h"

LightHandler::LightHandler(UINT winWidth, UINT winHeight)
	:shadowHeight(winHeight), shadowWidth(winWidth)
{
}

void LightHandler::addLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, DirectX::XMFLOAT3 UpDir, int type, float coneAngle)
{
	//Skapa ljus
	Light templight( color, position, direction, UpDir, coneAngle, type);
	this->lights.push_back(templight);
}
