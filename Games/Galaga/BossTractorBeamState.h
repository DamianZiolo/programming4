#pragma once

#include "BossState.h"

#include <memory>

namespace dae
{
    class BossTractorBeamState final : public BossState
    {
    public:
        BossTractorBeamState() = default;
        ~BossTractorBeamState() override = default;

        void OnEnter(EnemyBoss& boss) override;
        void OnExit(EnemyBoss& boss) override;

        std::unique_ptr<BossState> Update(EnemyBoss& boss) override;

    private:
        float m_Timer{};
        float m_BeamDelay{ 1.0f };
        float m_BeamDuration{ 4.0f };

        bool m_BeamEnabled{ false };
    };
}