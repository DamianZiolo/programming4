#include "EnemyButterflyReturnState.h"

#include "EnemyButterfly.h"
#include "EnemyButterflyFormationState.h"

namespace dae
{
	void EnemyButterflyReturnState::OnEnter(EnemyButterfly&)
	{
	}

	std::unique_ptr<EnemyButterflyState> EnemyButterflyReturnState::Update(EnemyButterfly& enemy)
	{
		if (enemy.ReturnToSlot(m_ReturnSpeed))
		{
			return std::make_unique<EnemyButterflyFormationState>();
		}

		return nullptr;
	}
}