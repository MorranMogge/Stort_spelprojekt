#include "stdafx.h"
#include "MouseEvent.h"

MouseEvent::MouseEvent()
	:type(EventType::INVALID), x(0), y(0)
{
}


MouseEvent::MouseEvent(const EventType type, const int x, const int y)
	:type(type), x(x), y(y)
{
}

MouseEvent::EventType MouseEvent::GetType() const
{
	return this->type;
}

int MouseEvent::GetPosX() const
{
	return this->x;
}

int MouseEvent::GetPosY() const
{
	return this->y;
}