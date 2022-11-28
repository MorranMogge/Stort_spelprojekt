#pragma once
#include "Sound.h"

struct SoundLibrary
{
	inline static Sound clickSfx;
	inline static Sound menuMusic;
	inline static Sound baseballbatSfx;
	SoundLibrary()
	{
		clickSfx.load(L"../Sounds/pickupCoin.wav");
		menuMusic.load(L"../Sounds/menuMusic.wav");
		baseballbatSfx.load(L"../Sounds/hitHurt.wav");
	}
};