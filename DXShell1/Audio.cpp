#include "Audio.h"
#include <cassert>
#include "GameController.h"


Audio* AudioLocator::_audio;

DirectXAudio::DirectXAudio(HWND windowHandle)
{
	_soundClass = new SoundClass();
	_soundClass->Initialize(windowHandle);

	_crossfadeIn = nullptr;
	_crossfadeOut = nullptr;

	//Effects
	_explosionSprite = new AudioSprite("Sounds/explosion.wav", _soundClass);
	_laserSprite = new AudioSprite("Sounds/shoot.wav", _soundClass);

	registerEffect(&_soundEffects, _explosionSprite);
	registerEffect(&_soundEffects, _laserSprite);

	//Theme Songs
	_mainThemeSprite = new AudioSprite("Sounds/main_theme.wav", _soundClass, true);
	_level2Theme = new AudioSprite("Sounds/old_friends_theme.wav", _soundClass, true);

	registerSong(&_songs, _mainThemeSprite);
	registerSong(&_songs, _level2Theme);
}

DirectXAudio::~DirectXAudio()
{
	stopAllSounds();

	for (AudioSprite* sprite : _songs) {
		delete sprite;
	}

	for (AudioSprite* sprite : _soundEffects) {
		delete sprite;
	}

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

AudioSprite::AudioSprite(char* filename, SoundClass* soundClass, bool playOnLoop)
{
	this->_soundClass = soundClass;

	this->_sound = nullptr;
	bool soundLoadedSuccessfully = _soundClass->LoadWaveFile(filename, &_sound);
	if (!soundLoadedSuccessfully) throw; //... make sure the .wav file is in proper format. Try re-converting using VLC

	this->playOnLoop = playOnLoop;
}

AudioSprite::~AudioSprite()
{
	ShutdownWaveFile(&_sound);
}

void AudioSprite::Play()
{
	if (this->playOnLoop) {
		_soundClass->PlayWaveFileOnLoop(_sound);
	}
	else {
		_soundClass->PlayWaveFile(_sound);
	}

}

void AudioSprite::Stop()
{
	_soundClass->StopWaveFile(_sound);
}

long AudioSprite::GetVolume()
{
	long output = -1;

	HRESULT result = _sound->GetVolume(&output);
	assert(result == DS_OK);

	return output;
}

void AudioSprite::SetVolume(long vol)
{
	if (vol < DSBVOLUME_MIN) { vol = DSBVOLUME_MIN; }
	else if (vol > DSBVOLUME_MAX) { vol = DSBVOLUME_MAX; }
	else {/* Volume in safe range. */ }


	HRESULT result = _sound->SetVolume(vol);
	assert(result == DS_OK);
}

bool AudioSprite::IsPlaying()
{
	LPDWORD status = new DWORD;
	_sound->GetStatus(status);

	bool output = ((*status) & DSBSTATUS_PLAYING) == 1;

	delete status;

	return output;
}

void DirectXAudio::stopAllSounds()
{
	for (AudioSprite* sprite : _songs) {
		sprite->Stop();
	}

	for (AudioSprite* sprite : _soundEffects) {
		sprite->Stop();
	}
}

void DirectXAudio::Update()
{


	//Cross-fade sounds if necessary.
	if (_crossfadeIn != nullptr || _crossfadeOut != nullptr) {

		//Get delta time
		using namespace std::chrono;
		time_point<steady_clock> currentTime = steady_clock::now();
		float elapsedTimeInMS = GameController::GetDeltaTime(); //duration<float, std::milli>(currentTime - timeOfRevival).count();



		if (_crossfadeIn != nullptr) {

			if (_crossfadeIn->GetVolume() <= kCrossFadeMax) {

				_crossfadeIn->SetVolume(_crossfadeIn->GetVolume() + static_cast<long>(kCrossFadeInStep * elapsedTimeInMS));

			}
			else {
				_crossfadeIn = nullptr;
			}
		}

		if (_crossfadeOut != nullptr) {

			if (_crossfadeOut->GetVolume() >= kCrossFadeMin) {

				_crossfadeOut->SetVolume(_crossfadeOut->GetVolume() - static_cast<long>(kCrossFadeOutStep * elapsedTimeInMS));

			}
			else {
				_crossfadeOut->Stop();
				_crossfadeOut = nullptr;
			}
		}

	}
}

void DirectXAudio::restartSound(Sounds soundType)
{
	getSpriteFromType(soundType)->Play();
}

AudioSprite* DirectXAudio::getSpriteFromType(Sounds soundType)
{
	AudioSprite* output = nullptr;

	switch (soundType)
	{

	case MAIN_THEME:
		output = _mainThemeSprite;
		break;
	case EXPLOSION:
		output = _explosionSprite;
		break;
	case LASER:
		output = _laserSprite;
		break;
	case LEVEL2_THEME:
		output = _level2Theme;
		break;
	default:
		throw; //ERROR: Unexpected/Invalid Enum
		break;
	}
	return output;
}

void DirectXAudio::registerSong(std::vector<AudioSprite*> * songsList, AudioSprite * sound)
{
	songsList->push_back(sound);
}

void DirectXAudio::registerEffect(std::vector<AudioSprite*> * effectsList, AudioSprite * effect)
{
	effectsList->push_back(effect);
}

void DirectXAudio::changeSound(Sounds stopSound, Sounds startSound) {
	changeSound(getSpriteFromType(stopSound), getSpriteFromType(startSound));
}

void DirectXAudio::changeSound(AudioSprite * stopSound, AudioSprite * startSound)
{

	//If we're already trying to crossfade out a song, just stop it abruptly.
	if (this->_crossfadeOut != nullptr) {
		this->_crossfadeOut->Stop();
	}

	this->_crossfadeOut = stopSound; //TODO: change this to use a vector, so multiple songs can be faded out at once.
	this->_crossfadeIn = startSound;

	this->_crossfadeIn->Play();
	this->_crossfadeIn->SetVolume(kCrossFadeMin);


	//Reset CrossfadeTimer
	using namespace std::chrono;
	time_point<steady_clock> currentTime = steady_clock::now();
	crossFadeStartTime = currentTime;
}


void DirectXAudio::playSound(Sounds soundType)
{
	AudioSprite* sound = getSpriteFromType(soundType);

	if (sound->IsPlaying()) {
		//Do nothing.
	}
	else {

		if (std::find(this->_songs.begin(), this->_songs.end(), sound) != this->_songs.end()) {
			//It's a theme song. Crossfade instead.

			bool foundAPlayingSong = false;
			for (AudioSprite* nextSong : _songs) {
				if (nextSong != sound && nextSong->IsPlaying()) {
					foundAPlayingSong = true;
					changeSound(nextSong, sound);
				}
			}

			if (!foundAPlayingSong) {
				sound->Play();
			}
		}
		else {
			sound->Play(); // Will reset sound to beginning and max volume.
		}
	}
}

void DirectXAudio::stopSound(Sounds soundType)
{
	getSpriteFromType(soundType)->Stop();
}