#pragma once
#include "SoundSystemEngine.h"

class NullSoundSystem : public SoundSystem
{
public:
	~NullSoundSystem() override = default;
	void Play(const SoundId& id, const float& volume) override {}
	void Add(const SoundId& id, const std::string& filepath) override {}
	void Mute() override {}
};

class NullMusicSystem : public MusicSystem
{
public:
	~NullMusicSystem() override = default;
	void Play(MusicId id, bool loop) override {}
	void Add(MusicId id, const std::string& filePath) override {}
	void Pause() override {}
	void Resume() override {}
	void Stop() override {}
	bool IsPlaying() const override { return false; }
	void Mute() override {}
};

