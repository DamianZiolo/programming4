#pragma once

#include "BossState.h"
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
    class BossTractorLoopState final : public BossState
    {
    public:
        BossTractorLoopState() = default;
        ~BossTractorLoopState() override = default;

        void OnEnter(EnemyBoss& boss) override;
        std::unique_ptr<BossState> Update(EnemyBoss& boss) override;

    private:
        glm::vec3 m_LoopCenter{};

        float m_Timer{};
        float m_LoopDuration{ 1.25f };
    };
}