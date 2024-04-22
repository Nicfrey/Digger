#include "SoundSystem.h"

#include <iostream>
#include <SDL_mixer.h>
#include <vector>

class SoundSystem::SoundSystemImpl
{
public:
	SoundSystemImpl() = default;
	~SoundSystemImpl();
	SoundSystemImpl(const SoundSystemImpl& other) = delete;
	SoundSystemImpl(SoundSystemImpl&& other) = delete;
	SoundSystemImpl& operator=(const SoundSystemImpl& other) = delete;
	SoundSystemImpl& operator=(SoundSystemImpl&& other) = delete;
	void DoPlaySound(SoundId id);
	void DoPlaySound(const Sound& sound);
	void DoPauseAll();
	void DoResumeAll();
private:
	struct SoundSDL
	{
		Sound baseSound;
		Mix_Chunk* pSound;
	};
	std::vector<SoundSDL> m_Sounds;
};

SoundSystem::SoundSystemImpl::~SoundSystemImpl()
{
	for(SoundSDL& sound : m_Sounds)
	{
		Mix_FreeChunk(sound.pSound);
		sound.pSound = nullptr;
	}
}

void SoundSystem::SoundSystemImpl::DoPlaySound(SoundId id)
{
	const auto it{std::ranges::find_if(m_Sounds.begin(),m_Sounds.end(),[id](const SoundSDL& other)
	{
		return other.baseSound.id == id;
	})};
	if(it != m_Sounds.end())
	{
		Mix_PlayChannel(-1,it->pSound,0);
	}
}

void SoundSystem::SoundSystemImpl::DoPlaySound(const Sound& sound)
{
	const auto it{ std::ranges::find_if(m_Sounds.begin(),m_Sounds.end(),[sound](const SoundSDL& other)
	{
		return other.baseSound.id == sound.id;
	}) };
	if(it != m_Sounds.end())
	{
		Mix_PlayChannel(-1,it->pSound,0);
		return;
	}
	SoundSDL soundSDL{sound};
	soundSDL.pSound = Mix_LoadWAV(sound.filePath.c_str());
	if(soundSDL.pSound == nullptr)
	{
		std::cerr << "SoundSystem::SoundSystemImpl::DoPlaySound() > Error: " << Mix_GetError() << '\n';
		return;
	}
	m_Sounds.emplace_back(soundSDL);
	Mix_PlayChannel(-1, soundSDL.pSound, 0);
}

void SoundSystem::SoundSystemImpl::DoPauseAll()
{
	Mix_Pause(-1);
}

void SoundSystem::SoundSystemImpl::DoResumeAll()
{
	Mix_Resume(-1);
}

void SoundSystem::PlaySound(SoundId id)
{
	m_pImpl->DoPlaySound(id);
}

void SoundSystem::PlaySound(const Sound& sound)
{
	m_pImpl->DoPlaySound(sound);
}

void SoundSystem::PauseAll()
{
	m_pImpl->DoPauseAll();
}

void SoundSystem::ResumeAll()
{
	m_pImpl->DoResumeAll();
}

class MusicSystem::MusicSystemImpl
{
public:
	MusicSystemImpl() = default;
	~MusicSystemImpl();
	MusicSystemImpl(const MusicSystemImpl& other) = delete;
	MusicSystemImpl(MusicSystemImpl&& other) = delete;
	MusicSystemImpl& operator=(const MusicSystemImpl& other) = delete;
	MusicSystemImpl& operator=(MusicSystemImpl&& other) = delete;
	void DoPlayMusic(MusicId id, bool loop);
	void DoPlayMusic(const Music& music, bool loop);
	void DoPause();
	void DoResume();
	void DoStop();
private:
	struct MusicSDL
	{
		Music baseMusic;
		Mix_Music* pMusic;
	};
	std::vector<MusicSDL> m_Musics;
};

MusicSystem::MusicSystemImpl::~MusicSystemImpl()
{
	for(MusicSDL& music : m_Musics)
	{
		Mix_FreeMusic(music.pMusic);
		music.pMusic = nullptr;
	}
}

void MusicSystem::MusicSystemImpl::DoPlayMusic(MusicId id, bool loop)
{
	const auto it{ std::ranges::find_if(m_Musics.begin(),m_Musics.end(),[id](const MusicSDL& other)
	{
		return other.baseMusic.id == id;
	}) };
	if(it != m_Musics.end())
	{
		Mix_PlayMusic(it->pMusic,loop ? -1 : 1);
	}
}

void MusicSystem::MusicSystemImpl::DoPlayMusic(const Music& music, bool loop)
{
	const auto it{ std::ranges::find_if(m_Musics.begin(),m_Musics.end(),[music](const MusicSDL& other)
	{
		return other.baseMusic.id == music.id;
	}) };
	if (it != m_Musics.end())
	{
		Mix_PlayMusic(it->pMusic, loop ? -1 : 1);
	}
}

void MusicSystem::MusicSystemImpl::DoPause()
{
	Mix_PauseMusic();
}

void MusicSystem::MusicSystemImpl::DoResume()
{
	Mix_ResumeMusic();
}

void MusicSystem::MusicSystemImpl::DoStop()
{
	Mix_HaltMusic();
}

void MusicSystem::PlayMusic(MusicId id, bool loop)
{
	m_pImpl->DoPlayMusic(id,loop);
}

void MusicSystem::PlayMusic(const Music& music, bool loop)
{
	m_pImpl->DoPlayMusic(music,loop);
}

void MusicSystem::Pause()
{
	m_pImpl->DoPause();
}

void MusicSystem::Resume()
{
	m_pImpl->DoResume();
}

void MusicSystem::Stop()
{
	m_pImpl->DoStop();
}
