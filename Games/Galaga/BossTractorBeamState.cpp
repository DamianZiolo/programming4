#include "BossTractorBeamState.h"

#include "EnemyBoss.h"
#include "BossFormationState.h"
#include "GameObject.h"
#include "GameTime.h"
#include "BossReturnState.h"

namespace dae
{
    void BossTractorBeamState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;

        m_Timer = 0.f;
        m_BeamEnabled = true;

        // TODO:
        // turn on sprite beam
        // turn on collider beam
    }

    void BossTractorBeamState::OnExit(EnemyBoss& boss)
    {
        (void)boss;

        m_BeamEnabled = false;

        // TODO:
       // turn off sprite beam
       // turn off collider beam
    }

    std::unique_ptr<BossState> BossTractorBeamState::Update(EnemyBoss& boss)
    {
        (void)boss;
        const auto dt = GameTime::GetIntance().GetDeltaTime();
        m_Timer += dt;

        // Boss stay in the sam place
        // Draw beam using:
        //
        // auto bossPos = boss.GetOwner()->GetWorldPosition();
        //
        // beamX = bossPos.x
        // beamY = bossPos.y + offset
        //don't draw beam imediatly, wait 1 second first!

        if (m_Timer >= m_BeamDuration)
        {
            return std::make_unique<BossReturnState>();
        }

        return nullptr;
    }
}