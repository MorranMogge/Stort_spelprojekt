#pragma once
#include <chrono>
#include <thread>

class Time final
{
public:

	static float DeltaTimeInNanoseconds()
	{
		return std::chrono::duration<float, std::nano>(std::chrono::steady_clock::now() - lap).count();
	}
	static float DeltaTimeInMicroseconds()
	{
		return std::chrono::duration<float, std::micro>(std::chrono::steady_clock::now() - lap).count();
	}
	static float DeltaTimeInMiliseconds()
	{
		return std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - lap).count();
	}
	static float DeltaTimeInSeconds()
	{
		return delta;
	}

	static float CurrentTime()
	{
		return std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::steady_clock::now() - start).count();
	}

	static void Reset()
	{
		delta = 0;
	}

	static void Update()
	{
		Start(); //restart time
	}
	static void Start()
	{
		delta = std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::steady_clock::now() - lap).count();
		lap = std::chrono::steady_clock::now();

		static const bool once = []()
		{
			start = lap;
			return true;
		} ();

	}


private:

	inline static std::chrono::steady_clock::time_point lap;
	inline static std::chrono::steady_clock::time_point start;
	inline static float delta = 0;

};