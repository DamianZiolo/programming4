#include "BossBombingRunState.h"
#include <EnemyBoss.h>
#include <BossFormationState.h>
#include "GameObject.h"
#include "GameTime.h"
#include <BossReturnState.h>
#include <BossState.h>
#include <RenderComponent.h>

void dae::BossBombingRunState::OnEnter(EnemyBoss& boss)
{
	m_StartPos = boss.GetOwner()->GetLocalPosition();
}

std::unique_ptr<dae::BossState> dae::BossBombingRunState::Update(dae::EnemyBoss& boss)
{
    auto* owner = boss.GetOwner();
    auto pos = owner->GetLocalPosition();

    float dt = GameTime::GetIntance().GetDeltaTime();
    m_Timer += dt;

    const float speed = 100.f;
    const float amplitude = 50.f;
    const float frequency = 4.f;

    // fly down + circles
    pos.y += speed * dt;
    pos.x += std::sin(m_Timer * frequency) * amplitude * dt;

    // TODO: spawn bombs here

    // after time we're going back
    if (m_Timer >= 3.0f) //atack time
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