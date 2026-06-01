#pragma once

#include "Singleton.h"

#include <string>
#include <vector>

namespace dae
{
	struct HighScoreEntry
	{
		std::string name{};
		int score{};
	};

	class HighScoreManager final : public Singleton<HighScoreManager>
	{
		friend class Singleton<HighScoreManager>;

	public:
		void SaveScore(const std::string& name, int score);
		std::vector<HighScoreEntry> LoadTopScores(int count) const;

	private:
		HighScoreManager() = default;

		std::string m_FilePath{ "Data/highscores.txt" };
	};
}