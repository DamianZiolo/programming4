#pragma once

#include "Enemy.h"

#include <memory>

namespace dae
{
	class EnemyButterflyState;

	class EnemyButterfly final : public Enemy
	{
	public:
		explicit EnemyButterfly(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~EnemyButterfly() override;

		void Update() override;
		void ChangeState(std::unique_ptr<EnemyButterflyState> newState);

		EnemyButterfly(const EnemyButterfly&) = delete;
		EnemyButterfly(EnemyButterfly&&) = delete;
		EnemyButterfly& operator=(const EnemyButterfly&) = delete;
		EnemyButterfly& operator=(EnemyButterfly&&) = delete;

	private:
		std::unique_ptr<EnemyButterflyState> m_State{};
	};
}