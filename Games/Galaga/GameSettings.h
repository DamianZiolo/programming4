#pragma once

#include "Singleton.h"
#include "GameMode.h"

namespace dae
{
	class GameSettings final : public Singleton<GameSettings>
	{
		friend class Singleton<GameSettings>;

	public:
		void SetGameMode(GameMode mode) { m_GameMode = mode; }
		GameMode GetGameMode() const { return m_GameMode; }

	private:
		GameSettings() = default;

		GameMode m_GameMode{ GameMode::Solo };
	};
}