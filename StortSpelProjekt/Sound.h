#pragma once
#include <Audio.h>
#include "stdafx.h"

class Sound final
{
	inline static std::unique_ptr<DirectX::AudioEngine> audEngine;

	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> effectInstanss;

public:
	static void Update()
	{
		if (!audEngine->Update())
		{
			// No audio device is active
			if (audEngine->IsCriticalError())
			{

			}
		}
	}

	void load(const std::wstring path)
	{
		static bool once = []()
		{
			(void)CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AUDIO_ENGINE_FLAGS::AudioEngine_Default;
			audEngine = std::make_unique<DirectX::AudioEngine>(flags);
			return true;
		} ();

		soundEffect = std::make_unique<DirectX::SoundEffect>(audEngine.get(), path.c_str());
		effectInstanss = soundEffect->CreateInstance();
	}

	void play(bool loop = false)
	{
		effectInstanss->Play(loop);
	}
	void setVolume(float volume)	//volume range 0 to 1 float
	{
		effectInstanss->SetVolume(volume);
	}
	void stop()
	{
		effectInstanss->Stop();
	}

	void pause()
	{
		effectInstanss->Pause();
	}
	void resume()
	{
		effectInstanss->Resume();
	}

};

