#pragma once
#include <string>

#include "Singleton.h"

typedef unsigned int SoundId;
typedef unsigned int MusicId;

struct Sound
{
	SoundId id;
	std::string filePath;
};

struct Music
{
	MusicId id;
	std::string filePath;
};

class SoundSystem : public dae::Singleton<SoundSystem>
{
public:
	SoundSystem() = default;
	~SoundSystem() override = default;
	void PlaySound(SoundId id);
	void PlaySound(const Sound& sound);
	void PauseAll();
	void ResumeAll();
private:
	class SoundSystemImpl;
	SoundSystemImpl* m_pImpl;
};

class MusicSystem : public dae::Singleton<MusicSystem>
{
public:
	MusicSystem() = default;
	~MusicSystem() override = default;
	void PlayMusic(MusicId id, bool loop);
	void PlayMusic(const Music& music, bool loop);
	void Pause();
	void Resume();
	void Stop();
private:
	class MusicSystemImpl;
	MusicSystemImpl* m_pImpl;
};

