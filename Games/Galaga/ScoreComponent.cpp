#include "ScoreComponent.h"

#include "GameObject.h"
#include "GameActor.h"
#include "Enemy.h"
#include "Event.h"

#include <stdexcept>

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	: Component(owner)
{
	auto actor = GetOwner()->GetComponent<GameActor>();

	if (!actor)
	{
		throw std::runtime_error("ScoreComponent requires a GameActor component");
	}
}

void dae::ScoreComponent::Notify(Event)
{
}

void dae::ScoreComponent::Notify(Event event, GameObject* gameObject)
{
	if (event != Event::EnemyDied)
		return;

	if (!gameObject)
		return;

	auto* enemy = gameObject->GetComponent<Enemy>();

	if (!enemy)
		return;

	const int scoreToAdd =
		GetScoreForEnemy(
			enemy->GetEnemyType(),
			enemy->IsInFormation());

	AddScore(scoreToAdd);
}

void dae::ScoreComponent::AddScore(int amount)
{
	m_Score += amount;

	GetOwner()->GetComponent<GameActor>()->NotifyObservers(Event::ScoreChanged);

	if (m_Score >= 500)
	{
		GetOwner()->GetComponent<GameActor>()->NotifyObservers(Event::ScoreReached500);
	}
}

int dae::ScoreComponent::GetScoreForEnemy(
	EnemyType enemyType,
	bool isInFormation) const
{
	switch (enemyType)
	{
	case EnemyType::Fly:
		return isInFormation ? 50 : 100;

	case EnemyType::Butterfly:
		return isInFormation ? 80 : 160;

	case EnemyType::Boss:
		return isInFormation ? 150 : 400;

	default:
		return 0;
	}
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}