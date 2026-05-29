#include "BossDiveDownState.h"

#include "BossReturnState.h"
#include "EnemyBoss.h"
#include "GameObject.h"
#include "GameTime.h"

#include <cstdlib>

namespace dae
{
    void BossDiveDownState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;

        m_HorizontalSpeed = static_cast<float>((rand() % 81) - 40);
    }

    std::unique_ptr<BossState> BossDiveDownState::Update(EnemyBoss& boss)
    {
        auto* owner = boss.GetOwner();

        const auto dt = GameTime::GetIntance().GetDeltaTime();

        auto pos = owner->GetWorldPosition();

        pos.x += m_HorizontalSpeed * dt;
        pos.y += m_DownSpeed * dt;

        owner->SetWorldPosition(pos);

        constexpr float screenHeight = 576.f;
        constexpr float topOffset = 50.f;

        if (pos.y > screenHeight + 20.f)
        {
            owner->SetWorldPosition(pos.x, -topOffset, pos.z);
            return std::make_unique<BossReturnState>();
        }

        return nullptr;
    }
}