#pragma once

class MouseEvent
{
public:
	enum class EventType
	{
		RAW_MOVE, INVALID
	};

private:
	EventType type;
	int x, y;

public:
	MouseEvent();
	MouseEvent(const EventType type, const int x, const int y);
	EventType GetType() const;

	int GetPosX() const;
	int GetPosY() const;
};