#include "EnemyFlyFormationState.h"

#include "EnemyFly.h"
#include "EnemyFlyBombingState.h"
#include "GameTime.h"

namespace dae
{
	void EnemyFlyFormationState::OnEnter(EnemyFly&)
	{
		m_Timer = 0.f;
	}

	std::unique_ptr<EnemyFlyState> EnemyFlyFormationState::Update(EnemyFly& enemy)
	{
		if (enemy.IsAttackRequested())
		{
			enemy.ClearAttackRequest();
			enemy.StartAttack();
			return std::make_unique<EnemyFlyBombingState>();
		}

		return nullptr;
	}
}