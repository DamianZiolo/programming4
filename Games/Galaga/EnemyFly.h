#pragma once
#include "Enemy.h"
#include <memory>

namespace dae
{
	class BoxCollider;
	class EnemyFlyState;

	class EnemyFly final : public Enemy
	{
	public:
		explicit EnemyFly(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~EnemyFly() override;

		void Update() override;
		void ChangeState(std::unique_ptr<EnemyFlyState> newState);


		EnemyFly(const EnemyFly&) = delete;
		EnemyFly(EnemyFly&&) = delete;
		EnemyFly& operator=(const EnemyFly&) = delete;
		EnemyFly& operator=(EnemyFly&&) = delete;

		void RequestAttack() { m_AttackRequested = true; }
		bool IsAttackRequested() const { return m_AttackRequested; }

		void ClearAttackRequest() { m_AttackRequested = false; }

		void SetAttacking(bool attacking) { m_IsAttacking = attacking; }
		bool IsAttacking() const { return m_IsAttacking; }

	private:
		std::unique_ptr<EnemyFlyState> m_State{};

		bool m_AttackRequested{ false };
		bool m_IsAttacking{ false };
	};

}