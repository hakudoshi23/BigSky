#include "soundManager.h"

std::map<std::string, HSAMPLE> SoundManager::cache;

SoundManager::SoundManager(){
	BASS_Init(1, 44100, 0, 0, NULL);
}

SoundManager::~SoundManager(){
	this->stop();
	BASS_Free();
}

void SoundManager::playSound(std::string sound){
	this->playSound(sound, 0);
}

void SoundManager::playSound(std::string sound, int freq_delta){
	this->playSound(sound, freq_delta, false);
}

void SoundManager::playSound(std::string sound, int freq_delta, bool loop){
	this->playSound(sound, freq_delta, loop, 1.0f);
}

void SoundManager::playSound(std::string sound, int freq_delta, bool loop, float volume){
	this->play();

	HSAMPLE sample;
	std::map<std::string, HSAMPLE>::iterator it = this->cache.find(sound);
	if(it == this->cache.end()) {
		sample = BASS_SampleLoad(0, sound.c_str(), 0, 0, 3, 0);
		this->cache[sound] = sample;
	} else sample = it->second;

	float delta = freq_delta != 0 ? (rand() % (freq_delta * 2)) - freq_delta : 0;

	BASS_SAMPLE info;
	BASS_SampleGetInfo(sample, &info);
	info.volume = volume;
	info.freq = 44100 + delta;
	info.flags = loop ? BASS_SAMPLE_LOOP : 0;
	BASS_SampleSetInfo(sample, &info);

	BASS_ChannelPlay(BASS_SampleGetChannel(sample,false), FALSE);
}

void SoundManager::play(){
	BASS_Start();
}

void SoundManager::pause(){
	BASS_Pause();
}

void SoundManager::stop(){
	BASS_Stop();
}

SoundManager* SoundManager::getInstance(){
	static SoundManager* instance = new SoundManager();
	return instance;
}