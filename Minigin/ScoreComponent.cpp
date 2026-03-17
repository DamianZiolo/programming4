#include "ScoreComponent.h"
#include "GameObject.h"
#include "GameActor.h"
#include "Event.h"
#include <cassert>
#include <stdexcept>

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	: Component(owner)
{
	auto actor = GetOwner()->GetComponent<GameActor>();
	//assert(actor && "ScoreComponent requires a GameActor component on the same GameObject");
	if (!actor)
	{
		throw std::runtime_error("ScoreComponent requires a GameActor component");
	}
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

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}