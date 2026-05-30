#include "EnemyFlyReturnState.h"

#include "EnemyFly.h"
#include "EnemyFlyFormationState.h"

namespace dae
{
	void EnemyFlyReturnState::OnEnter(EnemyFly&)
	{
	}

	std::unique_ptr<EnemyFlyState> EnemyFlyReturnState::Update(EnemyFly& enemy)
	{
		if (enemy.ReturnToSlot(m_ReturnSpeed))
		{
			return std::make_unique<EnemyFlyFormationState>();
		}

		return nullptr;
	}
}