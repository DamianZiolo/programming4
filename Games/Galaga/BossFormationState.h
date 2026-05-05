#pragma once
#include "BossState.h"

namespace dae
{
    class BossFormationState final : public BossState
    {
    public:
        void OnEnter(EnemyBoss& boss) override;
        std::unique_ptr<dae::BossState> Update(EnemyBoss& boss) override;
        std::unique_ptr<dae::BossState> OnHit(EnemyBoss& boss) override;

    private:
        float m_Timer{};
        float m_TimeUntilAttack{ 3.0f };
    };
}