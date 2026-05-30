#include "EnemyButterflyBombingState.h"

#include "EnemyButterfly.h"
#include "EnemyButterflyDiveDownState.h"
#include "EnemyButterflyLoopDiveState.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ProjectilePoolComponent.h"
#include "ProjectileComponent.h"

#include <cstdlib>

namespace dae
{
	void EnemyButterflyBombingState::OnEnter(EnemyButterfly&)
	{
		m_HasShot = false;
		m_HorizontalSpeed = static_cast<float>((rand() % 81) - 40);
	}

	std::unique_ptr<EnemyButterflyState> EnemyButterflyBombingState::Update(EnemyButterfly& enemy)
	{
		auto* owner = enemy.GetOwner();

		const auto dt = GameTime::GetIntance().GetDeltaTime();

		auto pos = owner->GetWorldPosition();

		pos.x += m_HorizontalSpeed * dt;
		pos.y += m_DownSpeed * dt;

		owner->SetWorldPosition(pos);

		constexpr float shootLine = ScreenHeight - 300.f;

		if (!m_HasShot && pos.y >= shootLine)
		{
			m_HasShot = true;

			for (int i = 0; i < 3; ++i)
			{
				enemy.GetProjectilePool().SpawnProjectile(
					glm::vec3{ pos.x, pos.y + 20.f, 0.f },
					100.f,
					ProjectileOwner::Enemy
				);
			}

			if (rand() % 2 == 0)
			{
				return std::make_unique<EnemyButterflyDiveDownState>();
			}

			return std::make_unique<EnemyButterflyLoopDiveState>();
		}

		return nullptr;
	}
}