#include "EnemyButterflyDiveDownState.h"

#include "EnemyButterfly.h"
#include "EnemyButterflyReturnState.h"
#include "GameObject.h"
#include "GameTime.h"

#include <cstdlib>

namespace dae
{
	void EnemyButterflyDiveDownState::OnEnter(EnemyButterfly&)
	{
		m_HorizontalSpeed = static_cast<float>((rand() % 81) - 40);
	}

	std::unique_ptr<EnemyButterflyState> EnemyButterflyDiveDownState::Update(EnemyButterfly& enemy)
	{
		auto* owner = enemy.GetOwner();

		const auto dt = GameTime::GetIntance().GetDeltaTime();

		auto pos = owner->GetWorldPosition();

		pos.x += m_HorizontalSpeed * dt;
		pos.y += m_DownSpeed * dt;

		owner->SetWorldPosition(pos);

		if (enemy.TeleportToTopIfBelowScreen())
		{
			return std::make_unique<EnemyButterflyReturnState>();
		}

		return nullptr;
	}
}