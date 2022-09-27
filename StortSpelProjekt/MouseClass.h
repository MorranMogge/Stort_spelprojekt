#pragma once
#include "MouseEvent.h"
#include <queue>
#include <windows.h>
#include "Camera.h"

class MouseClass
{
private:
	std::queue<MouseEvent> eventBuffer;
	int x = 0;
	int y = 0;

	POINT mousePos = {};
	MSG* msg;
	RECT rect;
	POINT ul;
	POINT lr;

public:
	MouseClass();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();
	void onMouseRaw(int x, int y);
	void clearEvents();
	void handleEvents(HWND* window, Camera& camera);
};