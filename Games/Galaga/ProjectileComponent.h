#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace dae
{
	enum class ProjectileOwner
	{
		Player,
		Enemy,
		None
	};


	class ProjectileComponent final : public Component
	{
	public:
		ProjectileComponent(GameObject* owner);
		virtual void OnCollisionEnter(BoxCollider* other) override;
		void Update() override;
		bool HasHit() { return m_HasHit; }
		void SetVelocity(float velocity);
		void Activate(const glm::vec3& position, float velocity, ProjectileOwner ownerType);
		void Deactivate();
		bool IsInUse() const;
		bool IsOutsideScreen() const;
		ProjectileOwner GetProjectileOwner() const;
	private:
		GameObject* m_pOwner;
		glm::vec3 m_Velocity{ 0.f, -100.f, 0.f };
		bool m_HasHit{ false };
		bool m_IsInUse{ false };
		float m_Timer{ 0.f };

		
		ProjectileOwner m_OwnerType{ ProjectileOwner::None };


	};
}