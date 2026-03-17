#pragma once
namespace dae
{
	class SteamManager final 
	{
	public:
		static void Init();
		static void Shutdown();
		static void Update();
		static void UnlockAchievement(const char* achievementID);

	};

}