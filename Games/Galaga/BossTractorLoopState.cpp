#include "BossTractorLoopState.h"

#include "EnemyBoss.h"
#include "BossTractorMoveState.h"
#include "GameObject.h"
#include "GameTime.h"

#include <algorithm>
#include <cmath>

namespace dae
{
    void BossTractorLoopState::OnEnter(EnemyBoss& boss)
    {
        m_Timer = 0.f;
        m_LoopCenter = boss.GetOwner()->GetWorldPosition();
    }

    std::unique_ptr<BossState> BossTractorLoopState::Update(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        const auto dt = GameTime::GetIntance().GetDeltaTime();
        m_Timer += dt;

        const auto progress =
            std::clamp(m_Timer / m_LoopDuration, 0.f, 1.f);

        constexpr float radiusX = 60.f;
        constexpr float radiusY = 35.f;
        constexpr float fullCircle = 6.2831853f;

        const auto angle = progress * fullCircle;

        auto pos = m_LoopCenter;

        pos.x = m_LoopCenter.x + std::sin(angle) * radiusX;
        pos.y = m_LoopCenter.y + (1.f - std::cos(angle)) * radiusY;

        owner->SetWorldPosition(pos);

        if (progress >= 1.f)
        {
            return std::make_unique<BossTractorMoveState>();
        }

        return nullptr;
    }
}