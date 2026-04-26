#pragma once

#include "SoundSystem.h"
#include "SoundRequest.h"
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>

namespace dae
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		void Play(sound_id id, float volume) override;
		void RegisterSound(sound_id id, const std::string& filePath) override;

		SDLSoundSystem(const SDLSoundSystem&) = delete;
		SDLSoundSystem(SDLSoundSystem&&) = delete;
		SDLSoundSystem& operator =(const SDLSoundSystem&) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&&) = delete;

	private:
		void ProcessQueue();

		class Impl;
		std::unique_ptr<Impl> m_Impl;

		//std::queue<SoundRequest> m_Queue;
		//std::mutex m_Mutex;

		//std::thread m_WorkerThread;

		////Note:
		////atomic bool is safe bool to use between threads, this variable is thread-safe
		//std::atomic_bool m_IsRunning{ true };


	};

}