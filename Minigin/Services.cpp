#include "Services.h"
#include "SDL_mixer.h"
#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <queue>
#include <mutex>

class dae::SDL_SoundSystem::SDL_SoundSystemImpl final
{
public:
	SDL_SoundSystemImpl();

	void Play(soundId id);
	void Load(soundId id, const std::string& soundName, int volume, bool playAfterLoading = false);
	void CleanUp();

private:
	struct soundInfo
	{
		soundId id;
		int volume;
		std::string filePath;
		bool playAfterLoading;
	};

	const std::string m_DataPath;

	std::map<soundId, Mix_Chunk*> m_SoundsMap;
	std::vector<Mix_Chunk*> m_pSounds;
	std::queue<soundId> m_PlayQueue;
	std::queue<soundInfo> m_LoadQueue;

	std::unique_ptr<std::thread> m_pPlayQueueThread;
	std::unique_ptr<std::thread> m_pLoadQueueThread;

	bool m_IsActive;
	void PlayQueue();
	void LoadQueue();

	std::mutex m_Mutex;
	std::condition_variable m_PlayCV;
	std::condition_variable m_LoadCV;
};

dae::SDL_SoundSystem::SDL_SoundSystemImpl::SDL_SoundSystemImpl()
	: m_DataPath( "../Data/" )
	, m_IsActive( true )
{
	//intialize sdl_mixer here
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512) < 0)
		std::cout << "Failed to open audio" << std::endl;

	if (Mix_AllocateChannels(MIX_CHANNELS) < 0)	// start with 5 channels to play audio on.
		std::cout << "Failed to allocate channels" << std::endl;

	m_pPlayQueueThread = std::make_unique<std::thread>(&SDL_SoundSystemImpl::PlayQueue, this); //check if this works without using ptr. Maybe threads are not on stack of variables?
	m_pLoadQueueThread = std::make_unique<std::thread>(&SDL_SoundSystemImpl::LoadQueue, this); //check if this works without using ptr. Maybe threads are not on stack of variables?
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::Play(soundId id)
{
	std::scoped_lock lock{ m_Mutex };
	if (m_SoundsMap.contains(id))
	{
		m_PlayQueue.push(id);
		m_PlayCV.notify_one();
	}
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::Load(soundId id, const std::string& soundName, int volume, bool playAfterLoading)
{
	auto filePath = m_DataPath + soundName;

	soundInfo si{};
	si.id = id;
	si.volume = volume;
	si.filePath = filePath;
	si.playAfterLoading = playAfterLoading;

	std::scoped_lock lock{ m_Mutex };
	m_LoadQueue.push(si);
	m_LoadCV.notify_one();


//	#ifdef _DEBUG
//		if (m_SoundsMap.contains(id))
//			std::cout << "id: " << id << " was overwritten" << std::endl;
//#endif
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::CleanUp()
{
	std::unique_lock lock{ m_Mutex };
	m_IsActive = false;

	for (Mix_Chunk* pSound : m_pSounds)
	{
		Mix_FreeChunk(pSound);
	}
	m_SoundsMap.clear();

	lock.unlock();
	
	m_PlayCV.notify_one();
	m_LoadCV.notify_one();

	m_pLoadQueueThread->join();
	m_pPlayQueueThread->join();

	Mix_CloseAudio();
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::PlayQueue()
{
	while (m_IsActive)
	{
		std::queue<soundId> soundsToPlay;
		std::unique_lock lock{ m_Mutex };

		while (!m_PlayQueue.empty())
		{
			//get a copy of sounds to play
			//remove the current ones from SHARED QUEUE
			//this can probably use an algorithm
			for (size_t i{}; i < m_PlayQueue.size(); ++i)
			{
				soundId id = m_PlayQueue.front();
				m_PlayQueue.pop();

				soundsToPlay.emplace(id);
			}
			lock.unlock();

			for (size_t i{}; i < soundsToPlay.size(); ++i)
			{
				soundId id = soundsToPlay.front();
				soundsToPlay.pop();

				Mix_Chunk* pSound = m_SoundsMap[id];
				Mix_PlayChannel(-1, pSound, 0);
			}

			lock.lock();
		}
		m_PlayCV.wait(lock);
	}
}

void dae::SDL_SoundSystem::SDL_SoundSystemImpl::LoadQueue()
{
	while (m_IsActive)
	{
		std::unique_lock lock{ m_Mutex };

		while (!m_LoadQueue.empty())
		{
			soundInfo& si = m_LoadQueue.front();

			lock.unlock();

			Mix_Chunk* pSound = Mix_LoadWAV(si.filePath.c_str());
			pSound->volume = static_cast<Uint8>(si.volume);
			
			if (pSound != nullptr)
			{
				lock.lock();

				m_SoundsMap[si.id] = pSound;
				m_pSounds.emplace_back(pSound);

				if (si.playAfterLoading)
				{
					m_PlayQueue.push(si.id);
					m_PlayCV.notify_one();
				}

				m_LoadQueue.pop();
			}
			else
				std::cout << "Failed to load wave file" << std::endl;
		}
		m_LoadCV.wait(lock);
	}
}

dae::SDL_SoundSystem::SDL_SoundSystem()
	: m_pimpl{std::make_unique<SDL_SoundSystemImpl>()}
{
}

dae::SDL_SoundSystem::~SDL_SoundSystem()
{
	m_pimpl->CleanUp();
}

void dae::SDL_SoundSystem::Play(soundId id)
{
	m_pimpl->Play(id);
}

void dae::SDL_SoundSystem::Load(soundId id, const std::string& soundName, int volume)
{
	m_pimpl->Load(id, soundName, volume);
}

void dae::SDL_SoundSystem::LoadPlay(soundId id, const std::string& soundName, int volume)
{
	m_pimpl->Load(id, soundName, volume, true);
}

//CHECK IF CREATING THE UNIQUE_PTR TO THE NULL_SS WILL DELETE THE NULL_SS WHEN SS IS CHANGED
dae::NULL_SoundSystem dae::ServiceLocator::m_NullSoundSystem;
dae::SoundSystem* dae::ServiceLocator::m_pSoundSystem = &dae::ServiceLocator::m_NullSoundSystem;
