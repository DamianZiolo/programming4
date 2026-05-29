#include "EnemyFlyFormationState.h"

#include "EnemyFly.h"
#include "EnemyFlyBombingState.h"
#include "GameTime.h"

namespace dae
{
	void EnemyFlyFormationState::OnEnter(EnemyFly&)
	{
		m_Timer = 0.f;
		m_TimeUntilAttack = 3.f;
	}

	std::unique_ptr<EnemyFlyState> EnemyFlyFormationState::Update(EnemyFly&)
	{
		m_Timer += GameTime::GetIntance().GetDeltaTime();

		if (m_Timer >= m_TimeUntilAttack)
		{
			return std::make_unique<EnemyFlyBombingState>();
		}

		return nullptr;
	}
}