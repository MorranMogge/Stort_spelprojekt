
#include "player.h"

player::player()
{
	xPos = 20.0f;
	yPos = 30.0f;
	zPos = 40.0f;
}

float player::getposition(char whichPos) const
{
	switch (whichPos)
	{
	case('x'):
		return xPos;
		break;
	case('y'):
		return yPos;
		break;
	case('z'):
		return zPos;
		break;
	default:
		return 69.0f;
		break;
	}
}

void player::setMatrix(DirectX::XMFLOAT4X4 matrix)
{
	this->matrix = matrix;
}

DirectX::XMFLOAT4X4 player::getMatrix() const
{
	return this->matrix;
}

void player::setPosition(float pos[3])
{
	this->xPos = pos[0];
	this->yPos = pos[1];
	this->zPos = pos[2];
}

void player::setPosition(float x, float y, float z)
{
	this->xPos = x;
	this->yPos = y;
	this->zPos = z;
}
