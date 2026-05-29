#pragma once
#include "Enemy.h"

namespace dae
{
	class BoxCollider;

	class EnemyFly final : public Enemy
	{
	public:
		explicit EnemyFly(GameObject* owner, ProjectilePoolComponent& projectilePool);
		~EnemyFly() override = default;

		void Update() override;

		EnemyFly(const EnemyFly&) = delete;
		EnemyFly(EnemyFly&&) = delete;
		EnemyFly& operator=(const EnemyFly&) = delete;
		EnemyFly& operator=(EnemyFly&&) = delete;

	};

}