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
		explicit EnemyBoss(GameObject* owner);
		~EnemyBoss() override = default;

		void Update() override;

		void OnCollisionEnter(BoxCollider* other) override;

		EnemyBoss(const EnemyBoss&) = delete;
		EnemyBoss(EnemyBoss&&) = delete;
		EnemyBoss& operator=(const EnemyBoss&) = delete;
		EnemyBoss& operator=(EnemyBoss&&) = delete;

		void TakeDamage();
		int GetHealth() { return m_Health; }
		void ChangeState(std::unique_ptr<BossState> newState);

	private:
		std::unique_ptr<BossState> m_State;
		int m_Health{ 2 };

	};

}