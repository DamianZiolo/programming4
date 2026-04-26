#include "ServiceLocator.h"
#include <NullSoundSystem.h>

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_SoundSystem =
std::make_unique<dae::NullSoundSystem>();


dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_SoundSystem;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
	if (soundSystem)
	{
		m_SoundSystem = std::move(soundSystem);
	}
	else 
	{
		m_SoundSystem = std::make_unique<NullSoundSystem>();
	}
}
