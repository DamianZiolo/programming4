#pragma once

#include "BossState.h"

#include <glm/glm.hpp>
#include <memory>

namespace dae
{
    class BossBombingRunState final : public BossState
    {
    public:
        BossBombingRunState() = default;
        ~BossBombingRunState() override = default;

        void OnEnter(EnemyBoss& boss) override;
        std::unique_ptr<BossState> Update(EnemyBoss& boss) override;

    private:
        float m_Timer{};
        float m_AttackDuration{ 1.5f };

        float m_ProjectileTimer{};
        float m_ProjectileSpeedRate{ 0.5f };
        const float radiusX = 300.f;
        const float radiusY = 250.f;
        const float fullCircle = 6.2831853f;
    };
}