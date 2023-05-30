#pragma once
#include <memory>
#include <string>

namespace dae
{
	using soundId = unsigned short;

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;
		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem operator=(const SoundSystem& other) = delete;
		SoundSystem operator=(SoundSystem&& other) = delete;

		//Don't call Play() right after Load(). The sound might not be loaded yet. Use LoadPlay() for this
		virtual void Play(soundId id) = 0;
		virtual void Load(soundId id, const std::string& soundName, int volume = 64) = 0;
		virtual void LoadPlay(soundId id, const std::string& soundName, int volume = 64) = 0;
	};

	class SDL_SoundSystem final : public SoundSystem
	{
	public:
		SDL_SoundSystem();
		virtual ~SDL_SoundSystem();
		SDL_SoundSystem(const SoundSystem& other) = delete;
		SDL_SoundSystem(SoundSystem&& other) = delete;
		SDL_SoundSystem operator=(const SoundSystem& other) = delete;
		SDL_SoundSystem operator=(SoundSystem&& other) = delete;

		virtual void Play(soundId id) override;
		virtual void Load(soundId id, const std::string& soundName, int volume = 64) override;
		virtual void LoadPlay(soundId id, const std::string& soundName, int volume = 64) override;

	private:
		class SDL_SoundSystemImpl;
		std::unique_ptr<SDL_SoundSystemImpl> m_pimpl;
	};

	class NULL_SoundSystem final : public SoundSystem
	{
	public:
		NULL_SoundSystem() = default;
		virtual ~NULL_SoundSystem() = default;
		NULL_SoundSystem(const SoundSystem& other) = delete;
		NULL_SoundSystem(SoundSystem&& other) = delete;
		NULL_SoundSystem operator=(const SoundSystem& other) = delete;
		NULL_SoundSystem operator=(SoundSystem&& other) = delete;

		virtual void Play(soundId) override {};
		virtual void Load(soundId, const std::string&, int) override {};
		virtual void LoadPlay(soundId, const std::string&, int) override {};
	};

	class ServiceLocator final
	{
	public:
		static SoundSystem* GetSoundSystem() { return m_pSoundSystem; }

		static void RegisterSoundSystem(SoundSystem* ss)
		{
			//if ss == nullptr ... ? : make ss equal to the nullss
			//m_pSoundSystem.reset(ss);
			m_pSoundSystem = ss;
		}

		static void CleanUp()
		{
			if (dynamic_cast<NULL_SoundSystem*>(m_pSoundSystem) == nullptr)
				delete m_pSoundSystem;
		}

	private:
		//CHECK IF CREATING THE UNIQUE_PTR TO THE NULL_SS WILL DELETE THE NULL_SS WHEN SS IS CHANGED
		static SoundSystem* m_pSoundSystem;
		static NULL_SoundSystem m_NullSoundSystem;
	};
}