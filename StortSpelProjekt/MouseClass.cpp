#include "stdafx.h"
#include "MouseClass.h"
#include "GPU.h"
#include <directxmath.h>

MouseClass::MouseClass()
{
}

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

void MouseClass::handleEvents(HWND* window, Camera& camera)
{
	if (PeekMessage(msg, NULL, WM_MOUSEFIRST, WM_KEYLAST, PM_REMOVE))
	{
		TranslateMessage(msg);
		DispatchMessage(msg);
	}

	while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
	{
	}

	ZeroMemory(&mousePos, sizeof(POINT));
	if (GetCursorPos(&mousePos))
	{
		if (ScreenToClient(*window, &mousePos))
		{
		}
	}

	while (!this->EventBufferIsEmpty())
	{
		MouseEvent me = this->ReadEvent();

		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			camera.AdjustRotation((float)me.GetPosY() * .0025f, (float)me.GetPosX() * .0025f);
			GetClientRect(*window, &rect);
			ul.x = rect.left;
			ul.y = rect.top;

			lr.x = rect.right;
			lr.y = rect.bottom;

			MapWindowPoints(*window, nullptr, &ul, 1);
			MapWindowPoints(*window, nullptr, &lr, 1);

			rect.left = ul.x;
			rect.top = ul.y;

			rect.right = lr.x;
			rect.bottom = lr.y;

			ClipCursor(&rect);
		}
	}
}