#pragma once

#include "BossState.h"

#include <memory>

namespace dae
{
    class BossDiveDownState final : public BossState
    {
    public:
        BossDiveDownState() = default;
        ~BossDiveDownState() override = default;

        void OnEnter(EnemyBoss& boss) override;
        std::unique_ptr<BossState> Update(EnemyBoss& boss) override;

    private:
        float m_DownSpeed{ 180.f };
        float m_HorizontalSpeed{};
    };
}