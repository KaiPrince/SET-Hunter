#include "Audio.h"


Audio* AudioLocator::_audio;

DirectXAudio::DirectXAudio(HWND windowHandle)
{
	_soundClass = new SoundClass(); 
	_soundClass->Initialize(windowHandle);

	_mainThemeSprite = new AudioSprite("Sounds/main_theme.wav", _soundClass);
	_explosionSprite = new AudioSprite("Sounds/explosion.wav", _soundClass);
	_laserSprite = new AudioSprite("Sounds/shoot.wav", _soundClass);
}

DirectXAudio::~DirectXAudio()
{
	stopAllSounds();

	if (_mainThemeSprite) { delete _mainThemeSprite; }
	if (_explosionSprite) { delete _explosionSprite; }
	if (_laserSprite) { delete _laserSprite; }

	_soundClass->Shutdown();

}

bool AudioSprite::LoadWaveFile(char* filename)
{
	return _soundClass->LoadWaveFile(filename, &_sound);
}

void AudioSprite::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

AudioSprite::AudioSprite(char* filename, SoundClass* soundClass)
{
	this->_soundClass = soundClass;

	this->_sound = nullptr;
	bool soundLoadedSuccessfully = _soundClass->LoadWaveFile(filename, &_sound);
	if (!soundLoadedSuccessfully) throw; //... make sure the .wav file is in proper format. Try re-converting using VLC
}

AudioSprite::~AudioSprite()
{
	ShutdownWaveFile(&_sound);
}

void AudioSprite::Play()
{
	_soundClass->PlayWaveFile(_sound);
}

void DirectXAudio::playSound(int soundID)
{
}

void DirectXAudio::stopSound(int soundID)
{
}

void DirectXAudio::stopAllSounds()
{
	PlaySound(NULL, 0, 0);
}


void DirectXAudio::playSound(Sounds soundType)
{
	switch (soundType)
	{
	case Audio::MAIN_THEME:
		//PlaySound(TEXT("Sounds/main_theme.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		_mainThemeSprite->Play();
		break;
	case Audio::EXPLOSION:
		_explosionSprite->Play();
		//PlaySound(TEXT("Sounds/explosion.wav"), NULL, SND_ASYNC | SND_FILENAME);
		break;
	case Audio::LASER:
		PlaySound(TEXT("Sounds/shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
		break;	
	default:
		break;
	}
}
