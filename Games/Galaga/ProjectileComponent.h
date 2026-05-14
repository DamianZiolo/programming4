#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace dae
{
	class ProjectileComponent final : public Component
	{
	public:
		ProjectileComponent(GameObject* owner);
		virtual void OnCollisionEnter(BoxCollider* other) override;
		void Update() override;
		bool HasHit() { return m_HasHit; }
	private:
		GameObject* m_pOwner;
		glm::vec3 m_Velocity{ 0.f, -100.f, 0.f };

		bool m_HasHit{ false };
	};
}