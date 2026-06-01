#pragma once
#include "BossState.h"

namespace dae
{
    class BossFormationState final : public BossState
    {
    public:
        void OnEnter(EnemyBoss&) override;
        std::unique_ptr<dae::BossState> Update(EnemyBoss&) override;
        

    private:
        float m_Timer{};
        float m_TimeUntilAttack{ 3.0f };
    };
}