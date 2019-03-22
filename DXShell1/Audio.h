#pragma once
#include <Windows.h>
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")

/*
Class Name: Audio
Purpose: This class encapsulates all audio features of this program.
*/
class Audio
{
public:
	Audio();
	~Audio();

	void PlayThemeSong() { PlaySound(TEXT("music.wav"), NULL, SND_ASYNC | SND_FILENAME); }
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