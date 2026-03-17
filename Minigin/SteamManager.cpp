#include "SteamManager.h"
#include <stdexcept>

#if USE_STEAMWORKS
#pragma warning(push)
#pragma warning(disable:4996)
#include <steam_api.h>
#pragma warning(pop)
#endif

void dae::SteamManager::Init()
{
#if USE_STEAMWORKS
	if (!SteamAPI_Init())
	{
		throw std::runtime_error("Steam must be running!");
	}
#endif
}

void dae::SteamManager::Shutdown()
{
#if USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif
}

void dae::SteamManager::Update()
{
#if USE_STEAMWORKS
	SteamAPI_RunCallbacks();
#endif
}