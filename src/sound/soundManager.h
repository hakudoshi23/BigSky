#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "../includes.h"
#include "../framework.h"
#include <string>
#include <bass.h>
#include <map>

class SoundManager
{
public:
	static SoundManager* getInstance();
	static std::map<std::string, HSAMPLE> cache;

	~SoundManager();

	void playSound(std::string sound);
	void playSound(std::string sound, int freq_delta);
	void playSound(std::string sound, int freq_delta, bool loop);

	void play();
	void pause();
	void stop();
private:
	SoundManager();
};


#endif