#pragma once

#include "BossState.h"
#include "BossAttackType.h"
#include <glm/glm.hpp>
#include <memory>

namespace dae
{
    class BossAttackMoveState final : public BossState
    {
    public:
        explicit BossAttackMoveState(BossAttackType attackType = BossAttackType::Random);

        ~BossAttackMoveState() override = default;

        void OnEnter(EnemyBoss& boss) override;
        std::unique_ptr<BossState> Update(EnemyBoss& boss) override;

    private:
        glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t) const;

        glm::vec3 m_StartPos{};
        glm::vec3 m_TargetPos{};

        float m_Timer{};
        float m_MoveDuration{ 1.0f };
        BossAttackType m_AttackType{ BossAttackType::Random };
    };
}