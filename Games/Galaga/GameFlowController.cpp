#include "GameFlowController.h"

#include "GameActor.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "LevelManagerComponent.h"
#include "SceneManager.h"
#include "ScoreComponent.h"
#include "HighScoreManager.h"
#include "GameSettings.h"

dae::GameFlowController::GameFlowController(
	GameObject* owner,
	LevelManagerComponent* levelManager,
	ScoreComponent* scoreComponent)
	: Component(owner)
	, m_pLevelManager{ levelManager }
	, m_pScoreComponent{ scoreComponent }
{
}

void dae::GameFlowController::AddPlayer(GameActor* player)
{
	if (!player)
		return;

	player->AddObserver(this);

	m_Players.push_back(player);
	m_PlayerDead.push_back(false);
}

void dae::GameFlowController::Update()
{
	if (m_HasEnded)
		return;

	if (m_pLevelManager && m_pLevelManager->AreAllLevelsCompleted())
	{
		GoToEndScreen();
	}
}

void dae::GameFlowController::Notify(Event event)
{
	if (m_HasEnded)
		return;

	if (event != Event::PlayerDied)
		return;

	for (int i = 0; i < static_cast<int>(m_Players.size()); ++i)
	{
		auto* player = m_Players[i];

		if (!player)
			continue;

		auto* health = player->GetOwner()->GetComponent<HealthComponent>();

		if (health && health->GetHealth() <= 0)
		{
			m_PlayerDead[i] = true;
		}
	}

	if (AreAllPlayersDead())
	{
		GoToEndScreen();
	}
}

bool dae::GameFlowController::AreAllPlayersDead() const
{
	if (m_PlayerDead.empty())
		return false;

	for (bool dead : m_PlayerDead)
	{
		if (!dead)
			return false;
	}

	return true;
}

void dae::GameFlowController::GoToEndScreen()
{
	m_HasEnded = true;

	const int score =
		m_pScoreComponent
		? m_pScoreComponent->GetScore()
		: 0;

	const auto& name =
		GameSettings::GetInstance().GetPlayerName();

	GameSessionStats::GetInstance().SetPlayerName(name);
	GameSessionStats::GetInstance().SetFinalScore(score);
	HighScoreManager::GetInstance().SaveScore(GameSettings::GetInstance().GetPlayerName(),score);

	dae::SceneManager::GetInstance().SetActiveScene(2);
}