#include "HighScoreDisplayComponent.h"

#include "TextComponent.h"
#include "HighScoreManager.h"
#include "GameSessionStats.h"

#include <iomanip>
#include <sstream>

dae::HighScoreDisplayComponent::HighScoreDisplayComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::HighScoreDisplayComponent::AddText(TextComponent* text)
{
	if (!text)
		return;

	m_Texts.push_back(text);
}

void dae::HighScoreDisplayComponent::Update()
{
	if (m_HasRefreshed)
		return;

	m_HasRefreshed = true;

	Refresh();
}

void dae::HighScoreDisplayComponent::Refresh()
{
	const auto scores =
		HighScoreManager::GetInstance().LoadTopScores(5);

	const auto& stats =
		GameSessionStats::GetInstance();

	if (m_Texts.size() >= 1 && m_Texts[0])
	{
		m_Texts[0]->SetText(
			"Your Score: " +
			stats.GetPlayerName() +
			" " +
			std::to_string(stats.GetFinalScore()));
	}

	if (m_Texts.size() >= 2 && m_Texts[1])
	{
		m_Texts[1]->SetText(
			"Shots fired: " +
			std::to_string(stats.GetShotsFired()));
	}

	if (m_Texts.size() >= 3 && m_Texts[2])
	{
		m_Texts[2]->SetText(
			"Number of hits: " +
			std::to_string(stats.GetHits()));
	}

	if (m_Texts.size() >= 4 && m_Texts[3])
	{
		std::stringstream ss;
		ss << "Hit miss ratio: "
			<< std::fixed
			<< std::setprecision(1)
			<< stats.GetHitMissRatio()
			<< "%";

		m_Texts[3]->SetText(ss.str());
	}

	for (int i = 0; i < 5; ++i)
	{
		const int textIndex = i + 4;

		if (textIndex >= static_cast<int>(m_Texts.size()))
			return;

		if (!m_Texts[textIndex])
			continue;

		std::string text{};

		if (i < static_cast<int>(scores.size()))
		{
			text =
				std::to_string(i + 1) + ". " +
				scores[i].name + " " +
				std::to_string(scores[i].score);
		}
		else
		{
			text =
				std::to_string(i + 1) + ". ---- 0";
		}

		m_Texts[textIndex]->SetText(text);
	}
}