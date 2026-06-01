#include "BossTractorMoveState.h"

#include "BossTractorBeamState.h"
#include "EnemyBoss.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GameSettings.h"
#include <algorithm>

namespace dae
{
    BossTractorMoveState::BossTractorMoveState()
    {
    }

    void BossTractorMoveState::OnEnter(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        m_Timer = 0.f;

        m_StartPos = owner->GetWorldPosition();
        constexpr float bossDistanceFromBottom = 320.f;

        m_TargetPos = glm::vec3{
            m_StartPos.x,
            GameSettings::ScreenHeight - bossDistanceFromBottom,
            0.f
        };
    }
    std::unique_ptr<BossState> BossTractorMoveState::Update(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        const auto dt = GameTime::GetIntance().GetDeltaTime();
        m_Timer += dt;

        const auto progress =
            std::clamp(m_Timer / m_MoveDuration, 0.f, 1.f);

        const auto pos = Lerp(m_StartPos, m_TargetPos, progress);
        owner->SetWorldPosition(pos);

        if (progress >= 1.f)
        {
            return std::make_unique<BossTractorBeamState>();
        }

        return nullptr;
    }


    glm::vec3 BossTractorMoveState::Lerp(
        const glm::vec3& a,
        const glm::vec3& b,
        float t) const
    {
        t = std::clamp(t, 0.f, 1.f);
        return a + (b - a) * t;
    }
}