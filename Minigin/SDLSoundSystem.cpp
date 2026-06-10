#include "SDLSoundSystem.h"

#include <condition_variable>
#include <queue>
#include <sstream>
#include <thread>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace dae
{
	class SDLSoundSystem::Impl
	{
	public:
		Impl()
		{
			if (!MIX_Init())
			{
#if WIN32
				OutputDebugStringA("MIX_Init failed\n");
#endif
				return;
			}

			m_Mixer = MIX_CreateMixerDevice(
				SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
				nullptr);

			if (m_Mixer == nullptr)
			{
#if WIN32
				OutputDebugStringA("MIX_CreateMixerDevice failed\n");
#endif
				return;
			}

			m_WorkerThread = std::thread(&Impl::ProcessQueue, this);
		}

		~Impl()
		{
			{
				// protect shared state before stopping worker thread
				std::lock_guard<std::mutex> lock(m_Mutex);

				m_IsRunning = false;
			}

			// wake up worker thread so it can exit
			m_ConditionVariable.notify_one();

			if (m_WorkerThread.joinable())
			{
				// wait until worker thread finishes
				m_WorkerThread.join();
			}

			if (m_Mixer != nullptr)
			{
				MIX_StopAllTracks(m_Mixer, 0);
				MIX_DestroyMixer(m_Mixer);
				m_Mixer = nullptr;
			}

			for (auto& clip : m_AudioClips)
			{
				if (clip.second != nullptr)
				{
					MIX_DestroyAudio(clip.second);
				}
			}

			m_AudioClips.clear();

			MIX_Quit();
		}

		void RegisterSound(sound_id id, const std::string& filePath)
		{
			//like above, I'm protecting shared sound path because multiple threads may acces the sound registry and Lock prevetns data races. FOr example main thread wants to save and worker thread want to read
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_SoundPaths[id] = filePath;
		}

		void Play(sound_id id, float volume)
		{
			{
				// protect queue while adding a new request
				std::lock_guard<std::mutex> lock(m_Mutex);
				//const float finalVolume = m_IsMuted ? 0.f : volume * m_MasterVolume;
				m_Queue.push(SoundRequest{ id, volume });
			}

			// wake up worker thread immediately
			m_ConditionVariable.notify_one();
		}

		void ToggleMute()
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_IsMuted = !m_IsMuted;
		}

		bool IsMuted() const
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_IsMuted;
		}

	private:
		void ProcessQueue()
		{
			while (true)
			{
				SoundRequest request{};

				{
					std::unique_lock<std::mutex> lock(m_Mutex);

					// sleep until new work arrives, this is moore efficient than polling with sleep_for
					m_ConditionVariable.wait(
						lock,
						[this]()
						{
							return !m_Queue.empty() || !m_IsRunning;
						});

					// Exit thread when shutting down
					if (!m_IsRunning && m_Queue.empty())
					{
						return;
					}

					// Remove next request from queue
					request = m_Queue.front();
					m_Queue.pop();
				}

				// do not hold mutex while playing/loading audio - note from classes by Alex
				// Audio operations may be slow and would block other threads.
				PlaySound(request);
			}
		}

		void PlaySound(const SoundRequest& request)
		{
			MIX_Audio* audio = GetAudio(request.id);

			if (audio == nullptr)
			{
				return;
			}

			if (!MIX_PlayAudio(m_Mixer, audio))
			{
#if WIN32
				OutputDebugStringA("MIX_PlayAudio failed\n");
#endif
			}
		}

		MIX_Audio* GetAudio(sound_id id)
		{
			// Protect shared audio cache
			std::lock_guard<std::mutex> lock(m_Mutex);

			auto audioIt = m_AudioClips.find(id);
			if (audioIt != m_AudioClips.end())
			{
				return audioIt->second;
			}

			auto pathIt = m_SoundPaths.find(id);
			if (pathIt == m_SoundPaths.end())
			{
#if WIN32
				OutputDebugStringA("Sound id was not registered\n");
#endif
				return nullptr;
			}

			MIX_Audio* audio =
				MIX_LoadAudio(m_Mixer, pathIt->second.c_str(), true);

			if (audio == nullptr)
			{
#if WIN32
				std::stringstream ss;
				ss << "Could not load sound: "
					<< pathIt->second
					<< "\n";

				OutputDebugStringA(ss.str().c_str());
#endif
				return nullptr;
			}

			m_AudioClips[id] = audio;

			return audio;
		}

		MIX_Mixer* m_Mixer{ nullptr };

		std::queue<SoundRequest> m_Queue{};
		std::unordered_map<sound_id, std::string> m_SoundPaths{};
		std::unordered_map<sound_id, MIX_Audio*> m_AudioClips{};
		//I know I know, Mutable but change of the mutex is not changing logic state of sound system so we can change it in const
		mutable std::mutex m_Mutex{};
		std::condition_variable m_ConditionVariable{};
		std::thread m_WorkerThread{};

		bool m_IsRunning{ true };
		bool m_IsMuted{ false };
	};

	SDLSoundSystem::SDLSoundSystem()
		: m_Impl(std::make_unique<Impl>())
	{
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::RegisterSound(sound_id id, const std::string& filePath)
	{
		m_Impl->RegisterSound(id, filePath);
	}

	void SDLSoundSystem::Play(sound_id id, float volume)
	{

		m_Impl->Play(id, volume);
	}

	void dae::SDLSoundSystem::ToggleMute()
	{
		m_Impl->ToggleMute();
	}

	bool dae::SDLSoundSystem::IsMuted() const
	{
		return m_Impl->IsMuted();
	}


}
//IMPORTANT NOTE THREADING ON THE EXAM
// PLAYING THE SOUND WHILE THE LOCK IS STILL ACTIVE



//dae::SDLSoundSystem::SDLSoundSystem()
//{
//	m_WorkerThread = std::thread(&SDLSoundSystem::ProcessQueue, this); //we're passing function ponter
//}
//
//dae::SDLSoundSystem::~SDLSoundSystem()
//{
//	// stop the worker thread and wait for it to finish so it 
//	// doesn't crash when the object is destroyed
//	m_IsRunning = false;
//	if (m_WorkerThread.joinable())
//	{
//		m_WorkerThread.join();
//	}
//}
//
//void dae::SDLSoundSystem::Play(sound_id id, float volume)
//{
//	std::lock_guard<std::mutex> lock(m_Mutex);
//	m_Queue.push(SoundRequest{ id,volume });
//
//}
//
//void dae::SDLSoundSystem::ProcessQueue()
//{
//	while (m_IsRunning)
//	{
//		SoundRequest request{};
//		bool hasRequest = false;
//		{
//			std::lock_guard<std::mutex> lock(m_Mutex);
//			if (!m_Queue.empty())
//			{
//				request = m_Queue.front();
//				m_Queue.pop();
//				hasRequest = true;
//			}
//
//		}
//
//		if (hasRequest)
//		{
//#if WIN32
//			std::stringstream ss;
//			ss << "Sound thread: play sound id"
//				<< request.id
//				<< "volume "
//				<< request.volume
//				<< "\n";
//			OutputDebugStringA(ss.str().c_str());
//#endif
//		}
//		else
//		{
//			std::this_thread::sleep_for(std::chrono::microseconds(1));
//		}
//
//	}
//
//}
