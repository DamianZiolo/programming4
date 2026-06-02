#pragma once

#include "Singleton.h"
#include <string>

namespace dae
{
	class GameSessionStats final : public Singleton<GameSessionStats>
	{
		friend class Singleton<GameSessionStats>;

	public:
		void Reset()
		{
			m_PlayerName = "AAAA";
			m_FinalScore = 0;
			m_ShotsFired = 0;
			m_Hits = 0;
		}

		void SetPlayerName(const std::string& name) { m_PlayerName = name; }
		const std::string& GetPlayerName() const { return m_PlayerName; }

		void SetFinalScore(int score) { m_FinalScore = score; }
		int GetFinalScore() const { return m_FinalScore; }

		void AddShot() { ++m_ShotsFired; }
		void AddHit() { ++m_Hits; }

		int GetShotsFired() const { return m_ShotsFired; }
		int GetHits() const { return m_Hits; }

		float GetHitMissRatio() const
		{
			if (m_ShotsFired <= 0)
				return 0.f;

			return static_cast<float>(m_Hits) / static_cast<float>(m_ShotsFired) * 100.f;
		}

	private:
		GameSessionStats() = default;

		std::string m_PlayerName{ "AAAA" };
		int m_FinalScore{};
		int m_ShotsFired{};
		int m_Hits{};
	};
}