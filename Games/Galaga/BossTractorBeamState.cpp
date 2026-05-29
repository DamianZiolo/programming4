#include "BossTractorBeamState.h"

#include "BossDiveDownState.h"
#include "EnemyBoss.h"
#include "GameTime.h"

namespace dae
{
    void BossTractorBeamState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;

        m_Timer = 0.f;
        m_BeamEnabled = false;
    }

    void BossTractorBeamState::OnExit(EnemyBoss& boss)
    {
        (void)boss;

        m_BeamEnabled = false;

        // TODO:
        // boss.EnableTractorBeam(false);
    }

    std::unique_ptr<BossState> BossTractorBeamState::Update(EnemyBoss& boss)
    {
        (void)boss;

        const auto dt = GameTime::GetIntance().GetDeltaTime();
        m_Timer += dt;

        if (!m_BeamEnabled && m_Timer >= m_BeamDelay)
        {
            m_BeamEnabled = true;

            // TODO:
            // boss.EnableTractorBeam(true);
        }

        if (m_Timer >= m_BeamDuration)
        {
            return std::make_unique<BossDiveDownState>();
        }

        return nullptr;
    }
}