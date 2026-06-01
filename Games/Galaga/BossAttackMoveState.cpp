#include "BossAttackMoveState.h"
#include "BossBombingRunState.h"
#include "BossTractorBeamState.h"
#include "EnemyBoss.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GameSettings.h"

#include <algorithm>
#include <cstdlib>

namespace dae
{
    void BossAttackMoveState::OnEnter(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        m_Timer = 0.f;
        m_StartPos = owner->GetWorldPosition();

        constexpr float minX = 100.f;
        constexpr float maxX = GameSettings::ScreenWidth - 100.f;
        constexpr float targetDistanceFromBottom = 300.f;

        const auto randomX =
            static_cast<float>(rand() % static_cast<int>(maxX - minX) + static_cast<int>(minX) );

        m_TargetPos = glm::vec3{
            randomX,
            GameSettings::ScreenHeight - targetDistanceFromBottom,
            0.f
        };
    }

    std::unique_ptr<BossState> BossAttackMoveState::Update(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        const auto dt = GameTime::GetIntance().GetDeltaTime();
        m_Timer += dt;

        const auto progress = std::clamp(m_Timer / m_MoveDuration, 0.f, 1.f);

        const auto pos = Lerp(m_StartPos, m_TargetPos, progress);
        owner->SetWorldPosition(pos);

        if (progress >= 1.f)
        {
            if (rand() % 2 == 0)
            {
                return std::make_unique<BossBombingRunState>();
            }

            return std::make_unique<BossTractorBeamState>();
        }

        return nullptr;
    }

    glm::vec3 BossAttackMoveState::Lerp(
        const glm::vec3& a,
        const glm::vec3& b,
        float t) const
    {
        t = std::clamp(t, 0.f, 1.f);
        return a + (b - a) * t;
    }
}