#pragma once
#include "BossState.h"

namespace dae
{
    class BossReturnState final : public BossState
    {
    public:
        void OnEnter(EnemyBoss& boss) override;
        std::unique_ptr<dae::BossState> Update(EnemyBoss& boss) override;
        std::unique_ptr<dae::BossState> OnHit(EnemyBoss& boss) override;

    private:
    };
}