#include "PlayerShootingComponent.h"

#include "GameObject.h"
#include "HealthComponent.h"
#include "ProjectilePoolComponent.h"
#include "ProjectileComponent.h"
#include "ServiceLocator.h"

#include <algorithm>

dae::PlayerShootingComponent::PlayerShootingComponent(
	GameObject* owner,
	ProjectilePoolComponent* projectilePool)
	: Component(owner)
	, m_pProjectilePool{ projectilePool }
{
}

void dae::PlayerShootingComponent::Shoot()
{
	auto* owner = GetOwner();

	if (!owner)
		return;

	if (!m_pProjectilePool)
		return;

	auto* health = owner->GetComponent<HealthComponent>();

	if (health && health->GetHealth() <= 0)
		return;

	RemoveInactiveProjectiles();

	if (static_cast<int>(m_ActiveProjectiles.size()) >= m_MaxProjectiles)
		return;

	if (ServiceLocator::GetSoundSystem().IsMuted() != true)
	{
		ServiceLocator::GetSoundSystem().Play(1, 1.f);
	}
	

	const auto ownerPos = owner->GetWorldPosition();

	auto* projectile = m_pProjectilePool->SpawnProjectile(
		glm::vec3{
			ownerPos.x,
			ownerPos.y,
			0.f
		},
		-250.f,
		ProjectileOwner::Player
	);

	if (projectile)
	{
		m_ActiveProjectiles.push_back(projectile);
	}
}

void dae::PlayerShootingComponent::RemoveInactiveProjectiles()
{
	for (auto it = m_ActiveProjectiles.begin(); it != m_ActiveProjectiles.end();)
	{
		auto* projectile = *it;

		if (projectile == nullptr ||
			!projectile->IsInUse() ||
			projectile->IsOutsideScreen() ||
			projectile->GetProjectileOwner() != ProjectileOwner::Player)
		{
			it = m_ActiveProjectiles.erase(it);
		}
		else
		{
			++it;
		}
	}
}