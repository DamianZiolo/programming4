#pragma once

#include "Singleton.h"
#include "GameMode.h"
#include <string>

namespace dae
{
	class GameSettings final : public Singleton<GameSettings>
	{
		friend class Singleton<GameSettings>;

	public:
		void SetGameMode(GameMode mode) { m_GameMode = mode; }
		GameMode GetGameMode() const { return m_GameMode; }
		void SetPlayerName(const std::string& name) { m_PlayerName = name; }
		const std::string& GetPlayerName() const { return m_PlayerName; }

	private:
		GameSettings() = default;
		std::string m_PlayerName{ "AAAA" };
		GameMode m_GameMode{ GameMode::Solo };
	};
}