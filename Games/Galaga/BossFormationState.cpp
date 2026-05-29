#include "BossFormationState.h"
#include "EnemyBoss.h"
#include "GameTime.h"
#include <BossBombingRunState.h>
#include "GameObject.h"
#include <RenderComponent.h>
#include <cstdlib>
#include "BossTractorLoopState.h"
//#include "BossBombingRunState.h"
//#include "BossTractorBeamState.h"

namespace dae
{
    void BossFormationState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;
        //reminder: Later here set sprite if you add animations!
        m_Timer = 0.f;
        m_TimeUntilAttack = 3.0f;
    }

    std::unique_ptr<dae::BossState> BossFormationState::Update(EnemyBoss& boss)
    {
        (void)boss;

        m_Timer += GameTime::GetIntance().GetDeltaTime();

        if (m_Timer >= m_TimeUntilAttack)
        {
            const auto randomAttack = rand() % 2;

            if (randomAttack == 0)
            {
                return std::make_unique<BossBombingRunState>();
            }

            return std::make_unique<BossTractorLoopState>();
        }

        return nullptr;
    }

}