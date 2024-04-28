#include "SoundSystemEngine.h"

#include <iostream>
#include <mutex>
#include <SDL_mixer.h>
#include <thread>
#include <vector>

#include "ResourceManager.h"

std::unique_ptr<SoundSystem> ServiceSoundLocator::m_SoundSystemInstance = nullptr;

class SoundSystemEngine::SoundSystemImpl
{
public:
	SoundSystemImpl() = default;
	~SoundSystemImpl();
	SoundSystemImpl(const SoundSystemImpl& other) = delete;
	SoundSystemImpl(SoundSystemImpl&& other) = delete;
	SoundSystemImpl& operator=(const SoundSystemImpl& other) = delete;
	SoundSystemImpl& operator=(SoundSystemImpl&& other) = delete;
	void DoAddSound(SoundId id, const std::string& filepath);
	void DoPlaySound(const SoundId& id, const float& volume);
	void DoPlaySound(const Sound& sound);
	void DoPauseAll();
	void DoResumeAll();

private:
	struct SoundSDL
	{
		SoundId id;
		Mix_Chunk* pSound;
	};

	std::vector<SoundSDL> m_Sounds{};
	std::jthread m_ThreadSound;
	std::mutex m_MutexSound;

	void PlaySoundThread(Mix_Chunk* sound, const float& volume);
	void LoadSoundThread(SoundId id, const std::string& filepath);
};

SoundSystemEngine::SoundSystemImpl::~SoundSystemImpl()
{
	for (SoundSDL& sound : m_Sounds)
	{
		Mix_FreeChunk(sound.pSound);
		sound.pSound = nullptr;
	}
}

void SoundSystemEngine::SoundSystemImpl::DoAddSound(SoundId id, const std::string& filepath)
{
	if (!m_Sounds.empty())
	{
		const auto it{
			std::ranges::find_if(m_Sounds.begin(), m_Sounds.end(), [id](const SoundSDL& other)
			{
				return other.id == id;
			})
		};
		if (it != m_Sounds.end())
		{
			return;
		}
	}
	if (m_ThreadSound.joinable())
		m_ThreadSound.join();

	if (!m_ThreadSound.joinable())
	{
		m_ThreadSound = std::jthread{
			&SoundSystemEngine::SoundSystemImpl::LoadSoundThread, this,id, filepath};
	}
}

void SoundSystemEngine::SoundSystemImpl::DoPlaySound(const SoundId& id, const float& volume)
{
	const auto it{
		std::ranges::find_if(m_Sounds.begin(), m_Sounds.end(), [id](const SoundSDL& other)
		{
			return other.id == id;
		})
	};
	if (it != m_Sounds.end())
	{
		if (m_ThreadSound.joinable())
			m_ThreadSound.join();

		if (!m_ThreadSound.joinable())
		{
			m_ThreadSound = std::jthread{
				&SoundSystemEngine::SoundSystemImpl::PlaySoundThread, this, it->pSound, volume
			};
		}
	}
}

void SoundSystemEngine::SoundSystemImpl::DoPlaySound(const Sound& sound)
{
	const auto it{
		std::ranges::find_if(m_Sounds.begin(), m_Sounds.end(), [sound](const SoundSDL& other)
		{
			return other.id == sound.id;
		})
	};
	if (it != m_Sounds.end())
	{
		Mix_PlayChannel(-1, it->pSound, 0);
		return;
	}
	SoundSDL soundSDL{sound.id, nullptr};
	soundSDL.pSound = Mix_LoadWAV(sound.filePath.c_str());
	if (soundSDL.pSound == nullptr)
	{
		std::cerr << "SoundSystemEngine::SoundSystemImpl::DoPlaySound() > Error: " << Mix_GetError() << '\n';
		return;
	}
	m_Sounds.emplace_back(soundSDL);
	Mix_PlayChannel(-1, soundSDL.pSound, 0);
}

void SoundSystemEngine::SoundSystemImpl::DoPauseAll()
{
	Mix_Pause(-1);
}

void SoundSystemEngine::SoundSystemImpl::DoResumeAll()
{
	Mix_Resume(-1);
}

void SoundSystemEngine::SoundSystemImpl::PlaySoundThread(Mix_Chunk* sound, const float& volume)
{
	std::lock_guard loadLock{m_MutexSound};
	Mix_VolumeChunk(sound, static_cast<int>(volume));
	Mix_PlayChannel(-1, sound, 0);
}

void SoundSystemEngine::SoundSystemImpl::LoadSoundThread(SoundId id, const std::string& filepath)
{
	std::lock_guard loadLock{m_MutexSound};
	const auto it{
		std::ranges::find_if(m_Sounds.begin(), m_Sounds.end(), [id](const SoundSDL& other)
		{
			return other.id == id;
		})
	};
	if (it == m_Sounds.end())
	{
		SoundSDL soundSDL{id, Mix_LoadWAV(std::string{ dae::ResourceManager::GetInstance().GetDataPath() + filepath }.c_str())};
		if (soundSDL.pSound == NULL)
		{
			std::cerr << "Can't load sound at : " << filepath << "with error: " << Mix_GetError() << "\n";
		}
		else
		{
			m_Sounds.emplace_back(soundSDL);
		}
	}
}

SoundSystem& ServiceSoundLocator::GetSoundSystem()
{
	return *m_SoundSystemInstance;
}

void ServiceSoundLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
{
	m_SoundSystemInstance = std::move(ss);
}

LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss): m_RealSoundSystem(std::move(ss))
{
}

void LoggingSoundSystem::Play(const SoundId& id, const float& volume)
{
	m_RealSoundSystem->Play(id, volume);
	std::cout << "Playing " << id << " at volume " << volume << "\n";
}

void LoggingSoundSystem::Add(const SoundId& id, const std::string& filepath)
{
	m_RealSoundSystem->Add(id, filepath);
	std::cout << "Added " << id << " at path " << filepath << "\n";
}

SoundSystemEngine::SoundSystemEngine()
{
	m_pImpl = new SoundSystemImpl{};
}

SoundSystemEngine::~SoundSystemEngine()
{
	delete m_pImpl;
	m_pImpl = nullptr;
}

void SoundSystemEngine::PlaySound(const Sound& sound)
{
	m_pImpl->DoPlaySound(sound);
}

void SoundSystemEngine::PauseAll()
{
	m_pImpl->DoPauseAll();
}

void SoundSystemEngine::ResumeAll()
{
	m_pImpl->DoResumeAll();
}

void SoundSystemEngine::Play(const SoundId& id, const float& volume)
{
	m_pImpl->DoPlaySound(id, volume);
}

void SoundSystemEngine::Add(const SoundId& id, const std::string& filepath)
{
	m_pImpl->DoAddSound(id, filepath);
}

class MusicSystemTest::MusicSystemImpl
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

MusicSystemTest::MusicSystemImpl::~MusicSystemImpl()
{
	for (MusicSDL& music : m_Musics)
	{
		Mix_FreeMusic(music.pMusic);
		music.pMusic = nullptr;
	}
}

void MusicSystemTest::MusicSystemImpl::DoPlayMusic(MusicId id, bool loop)
{
	const auto it{
		std::ranges::find_if(m_Musics.begin(), m_Musics.end(), [id](const MusicSDL& other)
		{
			return other.baseMusic.id == id;
		})
	};
	if (it != m_Musics.end())
	{
		Mix_PlayMusic(it->pMusic, loop ? -1 : 1);
	}
}

void MusicSystemTest::MusicSystemImpl::DoPlayMusic(const Music& music, bool loop)
{
	const auto it{
		std::ranges::find_if(m_Musics.begin(), m_Musics.end(), [music](const MusicSDL& other)
		{
			return other.baseMusic.id == music.id;
		})
	};
	if (it != m_Musics.end())
	{
		Mix_PlayMusic(it->pMusic, loop ? -1 : 1);
	}
}

void MusicSystemTest::MusicSystemImpl::DoPause()
{
	Mix_PauseMusic();
}

void MusicSystemTest::MusicSystemImpl::DoResume()
{
	Mix_ResumeMusic();
}

void MusicSystemTest::MusicSystemImpl::DoStop()
{
	Mix_HaltMusic();
}

void MusicSystemTest::PlayMusic(MusicId id, bool loop)
{
	m_pImpl->DoPlayMusic(id, loop);
}

void MusicSystemTest::PlayMusic(const Music& music, bool loop)
{
	m_pImpl->DoPlayMusic(music, loop);
}

void MusicSystemTest::Pause()
{
	m_pImpl->DoPause();
}

void MusicSystemTest::Resume()
{
	m_pImpl->DoResume();
}

void MusicSystemTest::Stop()
{
	m_pImpl->DoStop();
}
