#include "BossReturnState.h"
#include "BossFormationState.h"
#include "EnemyBoss.h"
#include "GameObject.h"
#include "GameTime.h"

#include <glm/glm.hpp>
#include <glm/geometric.hpp>

namespace dae
{
    void BossReturnState::OnEnter(EnemyBoss& boss)
    {
        (void)boss;
    }

    std::unique_ptr<BossState> BossReturnState::Update(EnemyBoss& boss)
    {
        GameObject* owner = boss.GetOwner();
        GameObject* slot = boss.GetSlot();

        if (!owner || !slot)
            return nullptr;

        const glm::vec3 currentPos = owner->GetWorldPosition();
        const glm::vec3 targetPos = slot->GetWorldPosition();

        constexpr float speed = 250.f;

        glm::vec3 direction = targetPos - currentPos;

        const float distance = glm::length(direction);

        if (distance > 0.001f)
        {
            direction = glm::normalize(direction);

            const glm::vec3 newPos =
                currentPos +
                direction *
                speed *
                GameTime::GetIntance().GetDeltaTime();

            owner->SetWorldPosition(newPos);
        }

        if (distance <= 2.f)
        {
            owner->SetParent(slot, false);
            owner->SetLocalPosition(0.f, 0.f, 0.f);
            boss.SetAttacking(false);
            return std::make_unique<BossFormationState>();
        }

        return nullptr;
    }

 
}