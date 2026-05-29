#include "EnemyFly.h"

#include "EnemyFlyFormationState.h"
#include "EnemyFlyState.h"

dae::EnemyFly::EnemyFly(GameObject* owner, ProjectilePoolComponent& projectilePool)
	: Enemy(owner, projectilePool)
{
	m_Health = 1;
	m_State = std::make_unique<EnemyFlyFormationState>();
}

dae::EnemyFly::~EnemyFly() = default;

void dae::EnemyFly::Update()
{
	if (!m_State)
		return;

	auto newState = m_State->Update(*this);

	if (newState)
	{
		ChangeState(std::move(newState));
	}
}

void dae::EnemyFly::ChangeState(std::unique_ptr<EnemyFlyState> newState)
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