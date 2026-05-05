#include "BossBombingRunState.h"
#include <EnemyBoss.h>
#include <BossFormationState.h>
#include "GameObject.h"
#include "GameTime.h"

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

    if (!m_Returning)
    {
        // fly down + circles
        pos.y += speed * dt;
        pos.x += std::sin(m_Timer * frequency) * amplitude * dt;

        // TODO: spawn bombs here

        // after time we're going back
        if (m_Timer >= 3.0f) //atack time
        {
            m_Returning = true;
        }
    }
    else
    {
        //going back to start position
        glm::vec3 dir = m_StartPos - pos;

        float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);

        if (distance < 5.f)
        {
            return std::make_unique<BossFormationState>();
        }

        dir /= distance;
        pos += dir * speed * dt;
    }

    owner->SetLocalPosition(pos);

    return nullptr;
}
