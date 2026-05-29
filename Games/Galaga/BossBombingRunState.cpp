#include "BossBombingRunState.h"

#include "BossDiveDownState.h"
#include "EnemyBoss.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ProjectilePoolComponent.h"
#include "ProjectileComponent.h"

#include <cmath>
#include <memory>

namespace dae
{
    void BossBombingRunState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;

        m_Timer = 0.f;
        m_ProjectileTimer = 0.f;
    }

    std::unique_ptr<BossState> BossBombingRunState::Update(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        const auto dt = GameTime::GetIntance().GetDeltaTime();

        m_Timer += dt;
        m_ProjectileTimer += dt;

        auto pos = owner->GetWorldPosition();

       

        const auto progress = std::min(m_Timer / m_AttackDuration, 1.f);
        const auto angle = progress * fullCircle;

        pos.x += std::cos(angle) * radiusX * dt;
        pos.y += std::sin(angle) * radiusY * dt;

        owner->SetWorldPosition(pos);

        if (m_ProjectileTimer >= m_ProjectileSpeedRate)
        {
            m_ProjectileTimer = 0.f;

            const auto worldPos = owner->GetWorldPosition();

            boss.GetProjectilePool().SpawnProjectile(
                glm::vec3{ worldPos.x, worldPos.y + 25.f, 0.f },
                100.f,
                ProjectileOwner::Enemy
            );
        }

        if (m_Timer >= m_AttackDuration)
        {
            return std::make_unique<BossDiveDownState>();
        }

        return nullptr;
    }
}