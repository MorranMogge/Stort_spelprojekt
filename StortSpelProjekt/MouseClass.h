#pragma once
#include "MouseEvent.h"
#include <queue>

class MouseClass
{
private:
	std::queue<MouseEvent> eventBuffer;
	int x = 0;
	int y = 0;

public:
	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();
	void onMouseRaw(int x, int y);
	void clearEvents();
};