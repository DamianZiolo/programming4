#include "BossBombingRunState.h"
#include <EnemyBoss.h>
#include <BossFormationState.h>
#include "GameObject.h"
#include "GameTime.h"
#include <BossReturnState.h>
#include <BossState.h>
#include <RenderComponent.h>
#include "ProjectilePoolComponent.h"

void dae::BossBombingRunState::OnEnter(EnemyBoss& boss)
{
	m_StartPos = boss.GetOwner()->GetLocalPosition();
    m_Timer = 0.f;
    m_ProjectileTimer = 0.f;
}

std::unique_ptr<dae::BossState> dae::BossBombingRunState::Update(dae::EnemyBoss& boss)
{
    auto* owner = boss.GetOwner();
    auto pos = owner->GetLocalPosition();

    const auto dt = GameTime::GetIntance().GetDeltaTime();
    m_Timer += dt;
    m_ProjectileTimer += dt;

    const auto speed = 100.f;
    const auto amplitude = 50.f;
    const auto frequency = 4.f;

    // fly down + circles
    pos.y += speed * dt;
    pos.x += std::sin(m_Timer * frequency) * amplitude * dt;

    //spawn bombs here
    if (m_ProjectileTimer >= m_ProjectileSpeedRate)
    {
        m_ProjectileTimer = 0.f;

        const auto worldPos = owner->GetWorldPosition();

        boss.GetProjectilePool().SpawnProjectile(
            glm::vec3{ worldPos.x, worldPos.y + 25.f, 0.f },
            100.f,
            ProjectileOwner::Enemy
        );
    }

    // after time we're going back
    if (m_Timer >= m_BombingTime) //atack time
    {
      return std::make_unique<BossReturnState>();
    }

    owner->SetLocalPosition(pos);
    return nullptr;
}




//RETURNING SHOULD BE REPLACE WITH SEPARATE STATE -> NOTES AT THE BOTTOM OF THE FILE
// 
//Class feedback:
// BossBombingRunState::Update(…)

// - replace float with auto and add {} variables initialization
// - returing supose to be the state instead of using ifs, because now     if (!m_Returning) behave like a state



//if (boss.GetHealth() <= 0)
//{
//    boss.Die();
//    return nullptr;
//}
//
////reminder: after 1st hit change sprite color of the boss here to purple;
//if (auto* render = boss.GetOwner()->GetComponent<RenderComponent>())
//{
//    render->SetTexture("boss_purple.png");
//}
//
//return nullptr;