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
		EnemyType GetEnemyType() const override { return EnemyType::Fly; }
		bool IsInFormation() const override { return !IsAttacking(); }

	private:
		std::unique_ptr<EnemyFlyState> m_State{};
	};
}