#include "EnemyFlyBombingState.h"

#include "EnemyFly.h"
#include "EnemyFlyReturnState.h"
#include "GameObject.h"
#include "GameTime.h"
#include "ProjectilePoolComponent.h"
#include "ProjectileComponent.h"

#include <cstdlib>

namespace dae
{
	void EnemyFlyBombingState::OnEnter(EnemyFly& enemy)
	{
		auto* owner = enemy.GetOwner();

		owner->SetParent(nullptr, true);

		m_HasShot = false;

		m_HorizontalSpeed = static_cast<float>((rand() % 81) - 40);
	}

	std::unique_ptr<EnemyFlyState> EnemyFlyBombingState::Update(EnemyFly& enemy)
	{
		auto* owner = enemy.GetOwner();

		const auto dt = GameTime::GetIntance().GetDeltaTime();

		auto pos = owner->GetWorldPosition();

		pos.x += m_HorizontalSpeed * dt;
		pos.y += m_DownSpeed * dt;

		owner->SetWorldPosition(pos);

		

		if (!m_HasShot && pos.y >= shootLine)
		{
			m_HasShot = true;

			enemy.GetProjectilePool().SpawnProjectile(
				glm::vec3{ pos.x, pos.y + 20.f, 0.f },
				100.f,
				ProjectileOwner::Enemy
			);
		}

		if (pos.y > ScreenHeight + 30.f)
		{
			constexpr float topOffset = 50.f;

			owner->SetWorldPosition(pos.x, -topOffset, pos.z);

			return std::make_unique<EnemyFlyReturnState>();
		}

		return nullptr;
	}
}