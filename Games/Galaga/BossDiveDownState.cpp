#include "BossDiveDownState.h"

#include "BossReturnState.h"
#include "EnemyBoss.h"
#include "GameObject.h"
#include "GameTime.h"
#include "GameSettings.h"
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

        if (pos.y > GameSettings::ScreenHeight + 20.f)
        {
            owner->SetWorldPosition(pos.x, -topOffset, pos.z);
            return std::make_unique<BossReturnState>();
        }

        return nullptr;
    }
}