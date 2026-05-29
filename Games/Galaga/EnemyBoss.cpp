#include "EnemyBoss.h"
#include "BossState.h"
#include <GameActor.h>
#include "EnemyBoss.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "GameActor.h"
#include <BossFormationState.h>
#include <RenderComponent.h>
#include "ProjectilePoolComponent.h"

dae::EnemyBoss::EnemyBoss(GameObject* owner, ProjectilePoolComponent& projectilePool):Enemy(owner, projectilePool)
{
	m_State = std::make_unique<BossFormationState>();
	m_Health = 2;
}

void dae::EnemyBoss::TakeDamage()
{
	Enemy::TakeDamage();

	if (m_Health == 1)
	{
		auto renderer = GetOwner()->GetComponent<RenderComponent>();
		renderer->SetTexture("boss_purple.png");
	}
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
