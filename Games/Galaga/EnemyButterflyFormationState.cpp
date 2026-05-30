#include "EnemyButterflyFormationState.h"

#include "EnemyButterfly.h"
#include "EnemyButterflyBombingState.h"

namespace dae
{
	void EnemyButterflyFormationState::OnEnter(EnemyButterfly&)
	{
	}

	std::unique_ptr<EnemyButterflyState> EnemyButterflyFormationState::Update(EnemyButterfly& enemy)
	{
		if (enemy.IsAttackRequested())
		{
			enemy.StartAttack();
			return std::make_unique<EnemyButterflyBombingState>();
		}

		return nullptr;
	}
}