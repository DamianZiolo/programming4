#include "BossTractorBeamState.h"

#include "EnemyBoss.h"
#include "BossFormationState.h"

#include <iostream>
#include <memory>

namespace dae
{
    void BossTractorBeamState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;
        m_Timer = 0.f;

        // TODO:
        // - set sprite animation
        // - stop boss in the middle of the screen
        // - Turn on collision for beam
        // boss.EnableTractorBeam(true);
    }

    void BossTractorBeamState::OnExit(EnemyBoss& boss)
    {
        (void)boss;
        // TODO:
        // - turn off beam
        // - turn off beam collider and animation
        // boss.EnableTractorBeam(false);
    }

    std::unique_ptr<BossState> BossTractorBeamState::Update(EnemyBoss& boss)
    {
        (void)boss;
        // TODO: 
        // m_Timer += Time::GetDeltaTime();
        // boss.UpdateTractorBeam();

        if (m_Timer >= m_BeamDuration)
        {
            return std::make_unique<BossFormationState>();
        }

        return nullptr;
    }

    std::unique_ptr<BossState> BossTractorBeamState::OnHit(EnemyBoss& boss)
    {
        boss.TakeDamage();
        return nullptr;
    }
}