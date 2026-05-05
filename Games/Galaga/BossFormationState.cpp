#include "BossFormationState.h"
#include "EnemyBoss.h"
#include "GameTime.h"
#include <BossBombingRunState.h>
#include "GameObject.h"
#include <RenderComponent.h>
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
            //later here do rand which state we're going into:
            return std::make_unique<BossBombingRunState>();
        }

        // reminder: add timer here and after time randomly pick one of the attacks
        // return new BossBombingRunState();
        // return new BossTractorBeamState();

        return nullptr;
    }

    std::unique_ptr<dae::BossState> BossFormationState::OnHit(EnemyBoss& boss)
    {
        if (boss.GetHealth() <= 0)
        {
            boss.Die(); 
            return nullptr;
        }

      //reminder: after 1st hit change sprite color of the boss here to purple;
        if (auto* render = boss.GetOwner()->GetComponent<RenderComponent>())
        {
            render->SetTexture("boss_purple.png");
        }

        return nullptr;
    }
}