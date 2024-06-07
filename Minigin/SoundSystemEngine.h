#pragma once
#include <memory>
#include <string>

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
	virtual void Mute() = 0;
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
	void Mute() override;

private:
	class SoundSystemImpl;
	SoundSystemImpl* m_pImpl;
};

class MusicSystem
{
public:
	virtual ~MusicSystem() = default;
	virtual void Play(MusicId id, bool loop) = 0;
	virtual void Add(MusicId id, const std::string& filePath) = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Stop() = 0;
	virtual bool IsPlaying() const = 0;
	virtual void Mute() = 0;
};

class MusicSystemEngine : public MusicSystem
{
public:
	MusicSystemEngine();
	~MusicSystemEngine() override;
	void Play(MusicId id, bool loop) override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	bool IsPlaying() const override;
	void Add(MusicId id, const std::string& filePath) override;
	void Mute() override;

private:
	class MusicSystemImpl;
	MusicSystemImpl* m_pImpl;
};


class ServiceSoundLocator final
{
public:
	static SoundSystem& GetSoundSystem();
	static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);
private:
	static std::unique_ptr<SoundSystem> m_SoundSystemInstance;
};

class LoggingSoundSystem final : public SoundSystem
{
public:
	LoggingSoundSystem() = delete;
	LoggingSoundSystem(const LoggingSoundSystem& other) = delete;
	LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss);
	~LoggingSoundSystem() override = default;
	void Play(const SoundId& id, const float& volume) override;
	void Add(const SoundId& id, const std::string& filepath) override;
	void Mute() override;
private:
	std::unique_ptr<SoundSystem> m_RealSoundSystem;
};

class ServiceMusicLocator final
{
public:
	static MusicSystem& GetMusicSystem();
	static void RegisterMusicSystem(std::unique_ptr<MusicSystem>&& ms);
private:
	static std::unique_ptr<MusicSystem> m_MusicSystemInstance;
};

class LoggingMusicSystem final : public MusicSystem
{
public:
	LoggingMusicSystem() = delete;
	LoggingMusicSystem(const LoggingMusicSystem& other) = delete;
	LoggingMusicSystem(std::unique_ptr<MusicSystem>&& ms);
	~LoggingMusicSystem() override = default;
	void Play(MusicId id, bool loop) override;
	void Pause() override;
	void Resume() override;
	void Stop() override;
	bool IsPlaying() const override;
	void Add(MusicId id, const std::string& filePath) override;
	void Mute() override;

private:
	std::unique_ptr<MusicSystem> m_RealMusicSystem;
};

