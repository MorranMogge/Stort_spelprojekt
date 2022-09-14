#include "MouseClass.h"
#include <string>

#include <directxmath.h>
#include <d3d11.h>

bool MouseClass::EventBufferIsEmpty()
{
	return eventBuffer.empty();
}

MouseEvent MouseClass::ReadEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		MouseEvent e = this->eventBuffer.front();
		this->eventBuffer.pop();
		return e;
	}
}

void MouseClass::onMouseRaw(int x, int y)
{
	return this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}

void MouseClass::clearEvents()
{
	for (int i = 0; i < eventBuffer.size(); i++)
	{
		this->eventBuffer.pop();
	}
}