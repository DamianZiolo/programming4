#include "EnemyFlyReturnState.h"

#include "EnemyFly.h"
#include "EnemyFlyFormationState.h"
#include "GameObject.h"
#include "GameTime.h"

#include <glm/glm.hpp>
#include <glm/geometric.hpp>

namespace dae
{
	void EnemyFlyReturnState::OnEnter(EnemyFly&)
	{
	}

	std::unique_ptr<EnemyFlyState> EnemyFlyReturnState::Update(EnemyFly& enemy)
	{
		auto* owner = enemy.GetOwner();
		auto* slot = enemy.GetSlot();

		if (!owner || !slot)
			return nullptr;

		const auto currentPos = owner->GetWorldPosition();
		const auto targetPos = slot->GetWorldPosition();

		auto direction = targetPos - currentPos;
		const auto distance = glm::length(direction);

		if (distance > 0.001f)
		{
			direction = glm::normalize(direction);

			const auto newPos =
				currentPos +
				direction *
				m_ReturnSpeed *
				GameTime::GetIntance().GetDeltaTime();

			owner->SetWorldPosition(newPos);
		}

		if (distance <= 2.f)
		{
			owner->SetParent(slot, false);
			owner->SetLocalPosition(0.f, 0.f, 0.f);

			return std::make_unique<EnemyFlyFormationState>();
		}

		return nullptr;
	}
}