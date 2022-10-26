#pragma once
#include <chrono>

struct TimeStruct
{
	std::chrono::time_point<std::chrono::system_clock> startTime;
	float dt;

	TimeStruct()
	{
		startTime = std::chrono::system_clock::now();
		dt = 0;
	}

	float getDt()
	{
		this->dt = ((std::chrono::duration<float>)(std::chrono::system_clock::now() - this->startTime)).count();
		return this->dt;
	}

	void resetStartTime()
	{
		startTime = std::chrono::system_clock::now();
	}

	bool getTimePassed(const float& timeInSeconds)
	{
		return timeInSeconds < ((std::chrono::duration<float>)(std::chrono::system_clock::now() - this->startTime)).count();
	}
};