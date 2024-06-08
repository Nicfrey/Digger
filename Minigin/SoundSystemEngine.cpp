#include "SoundSystemEngine.h"

#include <iostream>
#include <mutex>
#include <SDL_mixer.h>
#include <thread>
#include <vector>

#include "NullSoundSystem.h"
#include "ResourceManager.h"

std::unique_ptr<SoundSystem> ServiceSoundLocator::m_SoundSystemInstance = nullptr;
std::unique_ptr<MusicSystem> ServiceMusicLocator::m_MusicSystemInstance = nullptr;

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
	void DoMute();
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

void SoundSystemEngine::SoundSystemImpl::DoMute()
{
	Mix_Volume(-1, 0);
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
	if (m_SoundSystemInstance != nullptr)
	{
		m_SoundSystemInstance = std::make_unique<NullSoundSystem>();
	}
	else
	{
		m_SoundSystemInstance = std::move(ss);
	}
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

void LoggingSoundSystem::Mute()
{
	m_RealSoundSystem->Mute();
	std::cout << "Mute the sound\n";
}

MusicSystem& ServiceMusicLocator::GetMusicSystem()
{
	return *m_MusicSystemInstance;
}

void ServiceMusicLocator::RegisterMusicSystem(std::unique_ptr<MusicSystem>&& ms)
{
	if (m_MusicSystemInstance != nullptr)
	{
		m_MusicSystemInstance = std::make_unique<NullMusicSystem>();
	}
	else
	{
		m_MusicSystemInstance = std::move(ms);
	}
}

LoggingMusicSystem::LoggingMusicSystem(std::unique_ptr<MusicSystem>&& ms)
{
	m_RealMusicSystem = std::move(ms);
}

void LoggingMusicSystem::Play(MusicId id, bool loop)
{
	m_RealMusicSystem->Play(id, loop);
	std::cout << "Play " << id << "music\n";
}

void LoggingMusicSystem::Pause()
{
	std::cout << "Pause the current music\n";
	m_RealMusicSystem->Pause();
}

void LoggingMusicSystem::Resume()
{
	std::cout << "Resume the current music\n";
	m_RealMusicSystem->Resume();
}

void LoggingMusicSystem::Stop()
{
	std::cout << "Stop the current music\n";
	m_RealMusicSystem->Stop();
}

bool LoggingMusicSystem::IsPlaying() const
{
	if(m_RealMusicSystem->IsPlaying())
	{
		std::cout << "A music is currently playing\n";
		return true;
	}
	std::cout << "A music is not currently playing\n";
	return false;
}

void LoggingMusicSystem::Add(MusicId id, const std::string& filePath)
{
	std::cout << "Adding the music with id " << id << "from path: " << filePath << "\n";
	m_RealMusicSystem->Add(id, filePath);
}

void LoggingMusicSystem::Mute()
{
	std::cout << "Mute the music\n";
	m_RealMusicSystem->Mute();
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

void SoundSystemEngine::Mute()
{
	m_pImpl->DoMute();
}

class MusicSystemEngine::MusicSystemImpl
{
public:
	MusicSystemImpl() = default;
	~MusicSystemImpl();
	MusicSystemImpl(const MusicSystemImpl& other) = delete;
	MusicSystemImpl(MusicSystemImpl&& other) = delete;
	MusicSystemImpl& operator=(const MusicSystemImpl& other) = delete;
	MusicSystemImpl& operator=(MusicSystemImpl&& other) = delete;
	void DoAdd(MusicId id, const std::string& filepath);
	void DoPlayMusic(MusicId id, bool loop);
	void DoPlayMusic(const Music& music, bool loop);
	void DoPause();
	void DoResume();
	void DoStop();
	bool DoIsPlaying() const;
	void DoMute();

private:
	struct MusicSDL
	{
		Music baseMusic;
		Mix_Music* pMusic;
	};

	std::vector<MusicSDL> m_Musics;
};

MusicSystemEngine::MusicSystemImpl::~MusicSystemImpl()
{
	for (MusicSDL& music : m_Musics)
	{
		Mix_FreeMusic(music.pMusic);
		music.pMusic = nullptr;
	}
}

void MusicSystemEngine::MusicSystemImpl::DoAdd(MusicId id, const std::string& filepath)
{
	const auto it{
		std::ranges::find_if(m_Musics.begin(), m_Musics.end(), [id](const MusicSDL& other)
		{
			return other.baseMusic.id == id;
		})
	};
	if (it == m_Musics.end())
	{
		MusicSDL musicSDL{ Music{id,filepath},Mix_LoadMUS(std::string{ dae::ResourceManager::GetInstance().GetDataPath() + filepath }.c_str()) };
		if (musicSDL.pMusic == NULL)
		{
			std::cerr << "Can't load sound at : " << filepath << "with error: " << Mix_GetError() << "\n";
		}
		else
		{
			m_Musics.emplace_back(musicSDL);
		}
	}
}

bool MusicSystemEngine::MusicSystemImpl::DoIsPlaying() const
{
	return Mix_PlayingMusic();
}

void MusicSystemEngine::MusicSystemImpl::DoMute()
{
	Mix_VolumeMusic(0);
}

void MusicSystemEngine::MusicSystemImpl::DoPlayMusic(MusicId id, bool loop)
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

void MusicSystemEngine::MusicSystemImpl::DoPlayMusic(const Music& music, bool loop)
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

void MusicSystemEngine::MusicSystemImpl::DoPause()
{
	Mix_PauseMusic();
}

void MusicSystemEngine::MusicSystemImpl::DoResume()
{
	Mix_ResumeMusic();
}

void MusicSystemEngine::MusicSystemImpl::DoStop()
{
	Mix_HaltMusic();
}

MusicSystemEngine::MusicSystemEngine(): m_pImpl{new MusicSystemImpl{}}
{
}

MusicSystemEngine::~MusicSystemEngine()
{
	delete m_pImpl;
	m_pImpl = nullptr;
}

void MusicSystemEngine::Play(MusicId id, bool loop)
{
	m_pImpl->DoPlayMusic(id, loop);
}

void MusicSystemEngine::Pause()
{
	m_pImpl->DoPause();
}

void MusicSystemEngine::Resume()
{
	m_pImpl->DoResume();
}

void MusicSystemEngine::Stop()
{
	m_pImpl->DoStop();
}

bool MusicSystemEngine::IsPlaying() const
{
	return m_pImpl->DoIsPlaying();
}

void MusicSystemEngine::Add(MusicId id, const std::string& filePath)
{
	m_pImpl->DoAdd(id, filePath);
}

void MusicSystemEngine::Mute()
{
	m_pImpl->DoMute();
}
