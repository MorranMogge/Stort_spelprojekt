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
