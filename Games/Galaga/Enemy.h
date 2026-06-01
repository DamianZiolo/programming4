#pragma once

#include "Component.h"
#include "EnemyType.h"
namespace dae
{
	class BoxCollider;
	class ProjectilePoolComponent;

	class Enemy : public Component
	{
	public:
		explicit Enemy(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~Enemy() override = default;

		virtual void TakeDamage();

		Enemy(const Enemy&) = delete;
		Enemy(Enemy&&) = delete;
		Enemy& operator=(const Enemy&) = delete;
		Enemy& operator=(Enemy&&) = delete;

		void Die();

		void SetSlot(GameObject* slotPointer);
		GameObject* GetSlot() const { return m_pSlot; }

		ProjectilePoolComponent& GetProjectilePool() { return m_pProjectilePool; }

		void RequestAttack() { m_AttackRequested = true; }
		bool IsAttackRequested() const { return m_AttackRequested; }
		void ClearAttackRequest() { m_AttackRequested = false; }

		void StartAttack();
		void FinishAttack();

		bool IsAttacking() const { return m_IsAttacking; }

		bool TeleportToTopIfBelowScreen();

		bool ReturnToSlot(float speed);

		virtual EnemyType GetEnemyType() const = 0;
		virtual bool IsInFormation() const = 0;

	protected:
		int m_Health{};

	private:
		GameObject* m_pSlot{};
		ProjectilePoolComponent& m_pProjectilePool;

		bool m_AttackRequested{ false };
		bool m_IsAttacking{ false };
		static constexpr float TopOffset{ 50.f };
	};
}