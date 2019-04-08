#pragma once
#include <Windows.h>
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")

#include "soundclass.h"

/*
Class Name: Audio
Purpose: This interface class encapsulates all audio features of this program.
*/
class Audio
{
public:
	virtual ~Audio() {}

	enum Sounds
	{
		MAIN_THEME, EXPLOSION, LASER
	};

	virtual void playSound(int soundID) = 0; //Note: must be camel case, because of naming conflict with DirectX
	virtual void playSound(Sounds soundType) = 0;
	virtual void stopSound(int soundID) = 0;
	virtual void stopAllSounds() = 0;

	virtual void PlayThemeSong() { playSound(Sounds::MAIN_THEME); }
};

class DirectXAudio : public Audio
{
	SoundClass* _soundClass;
public:
	DirectXAudio(HWND windowHandle) { _soundClass = new SoundClass(); _soundClass->Initialize(windowHandle); }
	~DirectXAudio() { stopAllSounds(); _soundClass->Shutdown(); }

	// Inherited via Audio
	virtual void playSound(int soundID) override;
	virtual void playSound(Sounds soundType) override;

	virtual void stopSound(int soundID) override;

	virtual void stopAllSounds() override;

private:

	


};

/*
Class Name: AudioLocator
Purpose: This class implements the Service Locator pattern.
	This class allows the audio service to be accessed globally.
	Note: don't overuse this. Keep things SOLID.
*/
class AudioLocator
{
	AudioLocator(){}

	static Audio* _audio;
public:
	~AudioLocator(){}

	static Audio* GetAudio() { return _audio; }
	static void Provide(Audio* audio) { _audio = audio; }
};