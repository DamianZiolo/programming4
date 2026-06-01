#include "HighScoreDisplayComponent.h"

#include "TextComponent.h"
#include "HighScoreManager.h"

void dae::HighScoreDisplayComponent::AddText(TextComponent* text)
{
	if (!text)
		return;

	m_Texts.push_back(text);
}

dae::HighScoreDisplayComponent::HighScoreDisplayComponent(GameObject* owner)
	: Component(owner)
{
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

	for (int i = 0; i < static_cast<int>(m_Texts.size()); ++i)
	{
		if (!m_Texts[i])
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

		m_Texts[i]->SetText(text);
	}
}