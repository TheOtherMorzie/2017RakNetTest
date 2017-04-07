#pragma once

#include <mutex>
#include <map>

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>

#include "ExceptionC.h"

class SoundPlayer;

class SoundPlayerHandler
{
public:
	SoundPlayerHandler();
	~SoundPlayerHandler();

	// sigleton getters
	static std::mutex & getHandlerMutex();
	static SoundPlayerHandler & getHandler();

	// this will be added to a map, pairing the FMOD::Sound and SoundPlayer so that the SoundPlayer's static pcmreadcallback function can acess it's SoundPlayer (it's annoying I know)
	void addSoundSoundPlayerPair(FMOD::Sound * sound, SoundPlayer * soundPlayer);
	void removeSoundSoundPlayerPair(FMOD::Sound * sound);

	SoundPlayer * getSoundPlayer(FMOD::Sound * sound);
	std::map<FMOD::Sound*, SoundPlayer*> & getSoundSoundPlayerMap();

protected:

	std::map<FMOD::Sound*, SoundPlayer*> * m_sspMap;

};

