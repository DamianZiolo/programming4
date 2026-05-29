#pragma once
#include "Enemy.h"
#include <memory>
#include "BossState.h"
#include <EnemyBoss.h>

namespace dae
{
	class BoxCollider;
	class BossState;

	class EnemyBoss final : public Enemy
	{
	public:
		explicit EnemyBoss(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~EnemyBoss() override = default;
		void TakeDamage() override;

		void Update() override;
		EnemyBoss(const EnemyBoss&) = delete;
		EnemyBoss(EnemyBoss&&) = delete;
		EnemyBoss& operator=(const EnemyBoss&) = delete;
		EnemyBoss& operator=(EnemyBoss&&) = delete;

		int GetHealth() { return m_Health; }
		void ChangeState(std::unique_ptr<BossState> newState);

	private:
		std::unique_ptr<BossState> m_State;
		bool m_IsDamaged{ false };
	};

}