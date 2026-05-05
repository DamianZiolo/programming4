#include "EnemyBoss.h"
#include "BossState.h"
#include <GameActor.h>
#include "EnemyBoss.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "GameActor.h"
#include <BossFormationState.h>

dae::EnemyBoss::EnemyBoss(GameObject* owner):Enemy(owner)
{
	m_State = std::make_unique<BossFormationState>();
}

void dae::EnemyBoss::Update()
{
	if (m_State == nullptr)
	{
		return;
	}

	auto newState = m_State->Update(*this);

	if (newState)
	{
		ChangeState(std::move(newState));
	}

}

void dae::EnemyBoss::TakeDamage()
{
	if (m_State == nullptr)
		return;

	m_Health--;

	auto newState = m_State->OnHit(*this);

	if (newState)
	{
		ChangeState(std::move(newState));
	}
}

void dae::EnemyBoss::ChangeState(std::unique_ptr<BossState> newState)
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

void dae::EnemyBoss::OnCollisionEnter(BoxCollider* other)
{
	auto* actor = other->GetOwner()->GetComponent<GameActor>();

	if (actor)
	{
		TakeDamage();
	}
}