#include "SDLSoundSystem.h"

#include <chrono>
#include <sstream>

#if WIN32
#define WIN32_LEAN_AND_MEAN  //note: this mean we load less stuff
#include <windows.h>
#endif

dae::SDLSoundSystem::SDLSoundSystem()
{
	m_WorkerThread = std::thread(&SDLSoundSystem::ProcessQueue, this); //we're passing function ponter
}

dae::SDLSoundSystem::~SDLSoundSystem()
{
	// stop the worker thread and wait for it to finish so it 
	// doesn't crash when the object is destroyed
	m_IsRunning = false;
	if (m_WorkerThread.joinable())
	{
		m_WorkerThread.join();
	}
}

void dae::SDLSoundSystem::Play(sound_id id, float volume)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Queue.push(SoundRequest{ id,volume });

}

void dae::SDLSoundSystem::ProcessQueue()
{
	while (m_IsRunning)
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
#if WIN32
			std::stringstream ss;
			ss << "Sound thread: play sound id"
				<< request.id
				<< "volume "
				<< request.volume
				<< "\n";
			OutputDebugStringA(ss.str().c_str());
#endif
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}

	}

}
