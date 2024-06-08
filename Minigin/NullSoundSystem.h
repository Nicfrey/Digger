#pragma once
#include "SoundSystemEngine.h"

class NullSoundSystem final : public SoundSystem
{
public:
	~NullSoundSystem() override = default;
	void Play(const SoundId& id, const float& volume) override
	{
		(void)id;
		(void)volume;
	}
	void Add(const SoundId& id, const std::string& filepath) override
	{
		(void)id;
		(void)filepath;
	}
	void Mute() override {}
};

class NullMusicSystem final : public MusicSystem
{
public:
	~NullMusicSystem() override = default;
	void Play(MusicId id, bool loop) override
	{
		(void)id;
		(void)loop;
	}
	void Add(MusicId id, const std::string& filePath) override
	{
		(void)id;
		(void)filePath;
	}
	void Pause() override {}
	void Resume() override {}
	void Stop() override {}
	bool IsPlaying() const override { return false; }
	void Mute() override {}
};

