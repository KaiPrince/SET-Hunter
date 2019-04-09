#include "Audio.h"


Audio* AudioLocator::_audio;

DirectXAudio::DirectXAudio(HWND windowHandle)
{
	_soundClass = new SoundClass(); 
	_soundClass->Initialize(windowHandle);

	_mainThemeSprite = new AudioSprite("Sounds/main_theme.wav", _soundClass);
	_explosionSprite = new AudioSprite("Sounds/explosion.wav", _soundClass);
	_laserSprite = new AudioSprite("Sounds/shoot.wav", _soundClass);
	_level2Theme = new AudioSprite("Sounds/old_friends_theme.wav", _soundClass);
}

DirectXAudio::~DirectXAudio()
{
	stopAllSounds();

	if (_mainThemeSprite) { delete _mainThemeSprite; }
	if (_explosionSprite) { delete _explosionSprite; }
	if (_laserSprite) { delete _laserSprite; }
	if (_level2Theme) { delete _level2Theme; }

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

void AudioSprite::Stop()
{
	_soundClass->StopWaveFile(_sound);
}

void DirectXAudio::stopAllSounds()
{
	_mainThemeSprite->Stop();
	_explosionSprite->Stop();
	_laserSprite->Stop();
}


void DirectXAudio::playSound(Sounds soundType)
{
	switch (soundType)
	{
	case Audio::MAIN_THEME:
		_mainThemeSprite->Play();
		break;
	case Audio::EXPLOSION:
		_explosionSprite->Play();
		break;
	case Audio::LASER:
		_laserSprite->Play();
		break;	
	case Audio::LEVEL2_THEME:
		_level2Theme->Play();
		break;
	default:
		break;
	}
}

void DirectXAudio::stopSound(Sounds soundType)
{
	switch (soundType)
	{
	case Audio::MAIN_THEME:
		_mainThemeSprite->Stop();
		break;
	case Audio::EXPLOSION:
		_explosionSprite->Stop();
		break;
	case Audio::LASER:
		_laserSprite->Stop();
		break;
	default:
		break;
	}
}
