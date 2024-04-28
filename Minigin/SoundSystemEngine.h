#pragma once
#include <memory>
#include <string>

#include "Singleton.h"

using SoundId = unsigned short;
using MusicId = unsigned short;

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

class SoundSystem
{
public:
	virtual ~SoundSystem() = default;
	virtual void Play(const SoundId& id, const float& volume) = 0;
	virtual void Add(const SoundId& id, const std::string& filepath) = 0;
};

class SoundSystemEngine : public SoundSystem
{
public:
	SoundSystemEngine();
	~SoundSystemEngine() override;
	void PlaySound(const Sound& sound);
	void PauseAll();
	void ResumeAll();
	void Play(const SoundId& id, const float& volume) override;
	void Add(const SoundId& id, const std::string& filepath) override;

private:
	class SoundSystemImpl;
	SoundSystemImpl* m_pImpl;
};

class MusicSystemTest : public dae::Singleton<MusicSystemTest>
{
public:
	MusicSystemTest() = default;
	~MusicSystemTest() override = default;
	void PlayMusic(MusicId id, bool loop);
	void PlayMusic(const Music& music, bool loop);
	void Pause();
	void Resume();
	void Stop();
private:
	class MusicSystemImpl;
	MusicSystemImpl* m_pImpl;
};


class ServiceSoundLocator final
{
public:
	static SoundSystemEngine& GetSoundSystem();
	static void RegisterSoundSystem(std::unique_ptr<SoundSystemEngine>&& ss);
private:
	static std::unique_ptr<SoundSystemEngine> m_SoundSystemInstance;
};

