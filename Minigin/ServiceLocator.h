#pragma once
#include <memory>
#include "SoundSystem.h"

namespace dae
{
	//TODO: Swap to better sound system from presntation
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem> soundSystem);

	private:
		static std::unique_ptr<SoundSystem> m_SoundSystem;
	};

}