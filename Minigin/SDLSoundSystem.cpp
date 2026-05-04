#include "SDLSoundSystem.h"

#include <chrono>
#include <sstream>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN  //note: this mean we load less stuff
#include <windows.h>
#endif
#include <unordered_map>

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

			m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
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
			m_IsRunning = false;

			if (m_WorkerThread.joinable())
			{
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
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_SoundPaths[id] = filePath;
		}

		void Play(sound_id id, float volume)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Queue.push(SoundRequest{ id, volume });
		}

	private:
		void ProcessQueue() 
		{
			while (m_IsRunning) //use condition variable instead
			{
				SoundRequest request{};
				bool hasRequest = false;

				{
					std::lock_guard<std::mutex> lock(m_Mutex);

					if (!m_Queue.empty())
					{
						request = m_Queue.front();
						m_Queue.pop();
						hasRequest = true;
					}
				}

				if (hasRequest)
				{
					PlaySound(request);
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1)); //That much precision is not required
				}
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

			MIX_Audio* audio = MIX_LoadAudio(m_Mixer, pathIt->second.c_str(), true);
			if (audio == nullptr)
			{
#if WIN32
				std::stringstream ss;
				ss << "Could not load sound: " << pathIt->second << "\n";
				OutputDebugStringA(ss.str().c_str());
#endif
				return nullptr;
			}

			m_AudioClips[id] = audio;
			return audio;
		}

		MIX_Mixer* m_Mixer{ nullptr };

		std::queue<SoundRequest> m_Queue;
		std::unordered_map<sound_id, std::string> m_SoundPaths;
		std::unordered_map<sound_id, MIX_Audio*> m_AudioClips;

		std::mutex m_Mutex;
		std::thread m_WorkerThread;
		std::atomic_bool m_IsRunning{ true };
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
