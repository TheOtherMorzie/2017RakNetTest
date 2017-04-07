#include "SoundPlayerHandler.h"



SoundPlayerHandler::SoundPlayerHandler()
{
	m_sspMap = new std::map<FMOD::Sound*, SoundPlayer*>();
}


SoundPlayerHandler::~SoundPlayerHandler()
{
	if (m_sspMap != nullptr)
	{
		delete m_sspMap;
		m_sspMap = nullptr;
	}
}

std::mutex & SoundPlayerHandler::getHandlerMutex()
{
	static std::mutex handlerMutex;
	return handlerMutex;
}

SoundPlayerHandler & SoundPlayerHandler::getHandler()
{
	static SoundPlayerHandler handler;
	return handler;
}

void SoundPlayerHandler::addSoundSoundPlayerPair(FMOD::Sound * sound, SoundPlayer * soundPlayer)
{
	EVALUATEC(m_sspMap != nullptr);
	if (m_sspMap->count(sound) > 0)
	{
		(*m_sspMap)[sound] = soundPlayer;
	}
	else
	{
		m_sspMap->insert(std::pair<FMOD::Sound*, SoundPlayer*>(sound, soundPlayer));
	}
}

void SoundPlayerHandler::removeSoundSoundPlayerPair(FMOD::Sound * sound)
{
	EVALUATEC(m_sspMap != nullptr);
	m_sspMap->erase(sound);
}

SoundPlayer * SoundPlayerHandler::getSoundPlayer(FMOD::Sound * sound)
{
	SoundPlayer * sp = (*m_sspMap)[sound];
	return sp;
}

std::map<FMOD::Sound*, SoundPlayer*>& SoundPlayerHandler::getSoundSoundPlayerMap()
{
	EVALUATEC(m_sspMap != nullptr);
	return *m_sspMap;
}
