#pragma once
#include<string>

namespace dae
{
	using sound_id = unsigned short; // <- Remember, this is alias so sound_id and unsigned short are the same type

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default; //Must be virtual because without it when we delete ex.SDLSoundSystem only one desctructor would be called
		virtual void Play(sound_id id, float volume) = 0;
		virtual void RegisterSound(sound_id id, const std::string& filePath) = 0;
		
	};

}