#include "HighScoreManager.h"

#include <algorithm>
#include <fstream>

void dae::HighScoreManager::SaveScore(
	const std::string& name,
	int score)
{
	std::ofstream file{ m_FilePath, std::ios::app };

	if (!file.is_open())
		return;

	file << name << " " << score << "\n";
}

std::vector<dae::HighScoreEntry> dae::HighScoreManager::LoadTopScores(
	int count) const
{
	std::vector<HighScoreEntry> scores{};
	std::ifstream file{ m_FilePath };

	if (!file.is_open())
		return scores;

	std::string name{};
	int score{};

	while (file >> name >> score)
	{
		scores.push_back(HighScoreEntry{ name, score });
	}

	std::sort(
		scores.begin(),
		scores.end(),
		[](const HighScoreEntry& a, const HighScoreEntry& b)
		{
			return a.score > b.score;
		});

	if (static_cast<int>(scores.size()) > count)
	{
		scores.resize(count);
	}

	return scores;
}