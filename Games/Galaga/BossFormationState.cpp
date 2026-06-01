#include "BossFormationState.h"
#include "EnemyBoss.h"
#include "GameTime.h"
#include <BossBombingRunState.h>
#include "GameObject.h"
#include <RenderComponent.h>
#include <cstdlib>
#include "BossTractorLoopState.h"
#include <BossAttackMoveState.h>
#include "GameSettings.h"
#include "GameMode.h"
//#include "BossBombingRunState.h"
//#include "BossTractorBeamState.h"

namespace dae
{
    void BossFormationState::OnEnter(EnemyBoss&)
    {
        m_Timer = 0.f;
        m_TimeUntilAttack = 3.0f;
    }

    std::unique_ptr<dae::BossState> BossFormationState::Update(EnemyBoss&)
    {
		if (GameSettings::GetInstance().GetGameMode() == GameMode::Versus)
		{
			return nullptr;
		}

		m_Timer += GameTime::GetIntance().GetDeltaTime();

		if (m_Timer >= m_TimeUntilAttack)
		{
			return std::make_unique<BossAttackMoveState>(
				BossAttackType::Random);
		}

		return nullptr;
    }

}