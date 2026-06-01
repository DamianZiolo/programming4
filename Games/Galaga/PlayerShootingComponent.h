#pragma once

#include "Component.h"
#include <vector>

namespace dae
{
	class ProjectilePoolComponent;
	class ProjectileComponent;

	class PlayerShootingComponent final : public Component
	{
	public:
		PlayerShootingComponent(
			GameObject* owner,
			ProjectilePoolComponent* projectilePool);

		void Update() override {}

		void Shoot();

	private:
		void RemoveInactiveProjectiles();

		ProjectilePoolComponent* m_pProjectilePool{};

		std::vector<ProjectileComponent*> m_ActiveProjectiles{};

		int m_MaxProjectiles{ 2 };
	};
}