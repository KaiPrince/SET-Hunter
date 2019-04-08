#include "Audio.h"


Audio* AudioLocator::_audio;

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
		PlaySound(TEXT("Sounds/main_theme.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		break;
	case Audio::EXPLOSION:
		PlaySound(TEXT("Sounds/explosion.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP);
		break;
	case Audio::LASER:
		PlaySound(TEXT("Sounds/shoot.wav"), NULL, SND_ASYNC | SND_FILENAME);
		break;	
	default:
		break;
	}
}
