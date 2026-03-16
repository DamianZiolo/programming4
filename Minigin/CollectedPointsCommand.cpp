#include "CollectedPointsCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"

dae::CollectedPointsCommand::CollectedPointsCommand(GameObject* player, int points)
	: m_pTarget{ player }
	, m_Points{ points }
{
}
void dae::CollectedPointsCommand::Execute()
{
	auto score = m_pTarget->GetComponent<ScoreComponent>();

	if (score)
	{
		score->AddScore(m_Points);
	}
}