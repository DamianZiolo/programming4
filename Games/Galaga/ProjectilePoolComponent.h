#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include "ProjectileComponent.h"

namespace dae
{
	class Scene;
	class ProjectilePoolComponent final : public Component
	{
	public:
		ProjectilePoolComponent(GameObject* owner, int poolSize, Scene& scene);
		void Update() override {};
		ProjectileComponent* SpawnProjectile(
			const glm::vec3& position,
			float velocity,
			ProjectileOwner ownerType
		);

	private:
		std::vector<ProjectileComponent*> m_Projectiles;

	};
}