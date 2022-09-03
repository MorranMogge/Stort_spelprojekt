#pragma once

#include "Sound.h"

class SoundCollection final
{
public:

	inline static Sound testSound;

	static void Load()
	{
		testSound.load(L"../Sounds/itemAudio.wav");
	}

};