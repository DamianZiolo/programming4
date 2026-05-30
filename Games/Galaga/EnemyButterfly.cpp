#include "EnemyButterfly.h"

#include "EnemyButterflyFormationState.h"
#include "EnemyButterflyState.h"

dae::EnemyButterfly::EnemyButterfly(
	GameObject* owner,
	ProjectilePoolComponent& projectilePool)
	: Enemy(owner, projectilePool)
{
	m_Health = 1;
	m_State = std::make_unique<EnemyButterflyFormationState>();
}

dae::EnemyButterfly::~EnemyButterfly() = default;

void dae::EnemyButterfly::Update()
{
	if (!m_State)
		return;

	auto newState = m_State->Update(*this);

	if (newState)
	{
		ChangeState(std::move(newState));
	}
}

void dae::EnemyButterfly::ChangeState(
	std::unique_ptr<EnemyButterflyState> newState)
{
	if (m_State)
	{
		m_State->OnExit(*this);
	}

	m_State = std::move(newState);

	if (m_State)
	{
		m_State->OnEnter(*this);
	}
}