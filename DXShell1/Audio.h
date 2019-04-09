#pragma once
#include <Windows.h>
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")

#include "soundclass.h"
#include "Command.h"

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
		MAIN_THEME, EXPLOSION, LASER, LEVEL2_THEME
	};

	//Note: must be camel case, because of naming conflict with DirectX
	virtual void playSound(Sounds soundType) = 0;
	virtual void stopSound(Sounds soundType) = 0;
	virtual void changeSound(Sounds stopSound, Sounds startSound) = 0;
	virtual void stopAllSounds() = 0;

	virtual void PlayThemeSong() { playSound(Sounds::MAIN_THEME); }

	virtual void Update() = 0;
};

/*
Class Name: AudioLocator
Purpose: This class implements the Service Locator pattern.
	This class allows the audio service to be accessed globally.
	Note: don't overuse this. Keep things SOLID.
*/
class AudioLocator
{
	AudioLocator() {}

	static Audio* _audio;
public:
	~AudioLocator() {}

	static Audio* GetAudio() { return _audio; }
	static void Provide(Audio* audio) { _audio = audio; }
};

/*
Class Name: AudioSprite
Purpose: This class represents a sound in the game.
	It is the auditory equivalent of the SpriteSheet class.
*/
class AudioSprite
{
	SoundClass* _soundClass;

	IDirectSoundBuffer8* _sound;

	bool LoadWaveFile(char* filename);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

public:
	AudioSprite(char* filename, SoundClass* soundClass);
	~AudioSprite();

	void Play();
	void Stop();

	long GetVolume();
	void SetVolume(long vol);

};


/*
Class Name: DirectXAudio
Purpose: This is a concrete implementation of the audio interface, using DirectX.
*/
class DirectXAudio : public Audio
{
	const int kCrossFadeDurationInMS = 3000;
	const long kCrossFadeInStep = 100;
	const long kCrossFadeOutStep = 100;
	const long kCrossFadeMax = DSBVOLUME_MAX;
	const long kCrossFadeMin = DSBVOLUME_MIN;

	SoundClass* _soundClass;

	AudioSprite* _crossfadeOut;
	AudioSprite* _crossfadeIn;

	AudioSprite* _mainThemeSprite;
	AudioSprite* _explosionSprite;
	AudioSprite* _laserSprite;
	AudioSprite* _level2Theme;

	AudioSprite* getSpriteFromType(Sounds soundType);

	// Inherited via Audio
	virtual void changeSound(Sounds stopSound, Sounds startSound) override;


public:
	DirectXAudio(HWND windowHandle);
	~DirectXAudio();

	// Inherited via Audio
	virtual void playSound(Sounds soundType) override;

	virtual void stopSound(Sounds soundType) override;

	virtual void stopAllSounds() override;

	virtual void Update() override;

};